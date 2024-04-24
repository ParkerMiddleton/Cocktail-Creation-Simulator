#include "liquidmodel.h"

#include "glassware.h"

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QRandomGenerator>

#include <Box2D/Box2D.h>

LiquidModel::LiquidModel(QWidget *parent)
	: QObject{parent}

	, currentDash{""}

	, isMixing{false}
	, isPouring{false}
	, isDashing{false}
	, pouringElapsedTime{0}
	, scheduledLiquidPouringElapsedTime{0}

	, liquidPixmap{PIXMAP_WIDTH, PIXMAP_HEIGHT}
	, iceTexture{nullptr}
	, mintTexture{nullptr}

	, scheduleAddIceBody{false}
	, scheduleRemoveIceBodies{false}

	, scheduleAddMintBody{false}
	, scheduleRemoveMintBodies{false}

	, gravity{0.0f, -9.8f}
	, pouringSource{0.0f, 0.0f}

	, world{nullptr}
	, liquidParticles{nullptr}
	, dashParticles{nullptr}
{
	QPixmap iceImage(":/images/maindrink/icecube.png");
	iceTexture = new QPixmap(iceImage.scaled((ICE_RADIUS_M / CONVERSION_FACTOR) * 2, (ICE_RADIUS_M / CONVERSION_FACTOR) * 2, Qt::KeepAspectRatio, Qt::SmoothTransformation));

	QPixmap mintImage(":/images/maindrink/mintspring.png");
	mintTexture = new QPixmap(mintImage.scaled(((MINT_RADIUS_M * 3) / CONVERSION_FACTOR), ((MINT_RADIUS_M * 3) / CONVERSION_FACTOR), Qt::KeepAspectRatio, Qt::SmoothTransformation));

	uchar ALPHA = 200;

	// Setup colors for drinks.
	drinkColors["whiskey"] =					{165, 113, 10, ALPHA};				// Whiskey color
	drinkColors["tequila"] =					{172, 73, 25, ALPHA};				// Tequila color
	drinkColors["rum"] =						{236, 233, 226, ALPHA};				// Rum color
	drinkColors["vodka"] =						{236, 233, 226, 50};				// Vodka color
	drinkColors["gin"] =						{236, 233, 226, ALPHA};				// Gin color
	drinkColors["kahlua"] =						{28, 1, 2, ALPHA};					// kahlua
	drinkColors["burbon"] =						{165, 113, 10, ALPHA};				// burbon
	drinkColors["sweet and sour mix"] =			{210, 196, 112, ALPHA};				// sweet n sour
	drinkColors["grenadine"] =					{180, 19, 41, ALPHA};				// grenadine
	drinkColors["simple syrup"] =				{236, 233, 226, ALPHA};				// simpe syrup
	drinkColors["agave nectar"] =				{245, 183, 10, ALPHA};				// agave nectar
	drinkColors["sprite"] =						{236, 233, 226, ALPHA};				// sprite
	drinkColors["coke"] =						{56, 45, 43, ALPHA};				// coke
	drinkColors["half n half"] =				{255, 255, 255, ALPHA};				// half n half
	drinkColors["lime juice"] =					{158, 180, 80, ALPHA};				// lime juice
	drinkColors["ginger beer"] =				{253, 237, 115, ALPHA};				// ginger beer
	drinkColors["olive juice"] =				{207, 178, 112, ALPHA};				// olive juice
	drinkColors["prosecco"] =					{231, 225, 209, ALPHA};				// prosecco

	// Setup colors for dashes.
	dashColors["bitters"] =						{179, 102, 110};
	dashColors["orange liquor splash"] =		{210, 196, 112};

	// Setup liquid particle system.
	liquidParticlesDef.destroyByAge = true;
	liquidParticlesDef.colorMixingStrength = 0.1f;

	liquidParticlesDef.radius = LIQUID_PARTICLE_RADIUS_M;

	// Setup dash particle system.
	dashParticlesDef.destroyByAge = true;
	dashParticlesDef.radius = DASH_PARTICLE_RADIUS_M;
}

LiquidModel::~LiquidModel()
{
	delete iceTexture;
	delete mintTexture;

	this->removeGlassware();
}

void LiquidModel::updateGlassware(const Glassware &glassware)
{
	this->removeGlassware();

	world = new b2World(gravity);

	// Get source of pourings.
	pouringSource.x = glassware.getPhysicsPouringSource().x() * CONVERSION_FACTOR;
	pouringSource.y = (PIXMAP_HEIGHT - glassware.getPhysicsPouringSource().y()) * CONVERSION_FACTOR;

	// Get vertices of glassware collision.
	const QList<QPointF> &qVs = glassware.getPhysicsCollisionVertices();
	b2Vec2 collisionVertices[6];
	collisionVertices[0].Set(qVs[0].x() * CONVERSION_FACTOR, (PIXMAP_HEIGHT - qVs[0].y()) * CONVERSION_FACTOR);
	collisionVertices[1].Set(qVs[1].x() * CONVERSION_FACTOR, (PIXMAP_HEIGHT - qVs[1].y()) * CONVERSION_FACTOR);
	collisionVertices[2].Set(qVs[2].x() * CONVERSION_FACTOR, (PIXMAP_HEIGHT - qVs[2].y()) * CONVERSION_FACTOR);
	collisionVertices[3].Set(qVs[3].x() * CONVERSION_FACTOR, (PIXMAP_HEIGHT - qVs[3].y()) * CONVERSION_FACTOR);

	// Close the chain that will form the body.
	collisionVertices[4].Set(collisionVertices[2].x, collisionVertices[2].y);
	collisionVertices[5].Set(collisionVertices[1].x, collisionVertices[1].y);

	// Get ice and mint horizontal spawn range.
	bodiesHorizontalSpawnRange[0] = collisionVertices[0].x + (ICE_RADIUS_M * 2.0f); // Left Bound.
	bodiesHorizontalSpawnRange[1] = collisionVertices[3].x - (ICE_RADIUS_M * 2.0f); // Right Bound.

	// Construct collision body.
	b2BodyDef collisionBodyDef;
	b2Body *collisionBody = world->CreateBody(&collisionBodyDef);

	b2ChainShape collisionShape;
	collisionShape.CreateLoop(collisionVertices, 6);
	collisionBody->CreateFixture(&collisionShape, 0.0f);

	// Create particle systems.
	liquidParticles = world->CreateParticleSystem(&liquidParticlesDef);
	dashParticles = world->CreateParticleSystem(&liquidParticlesDef);
	dashParticles->SetRadius(DASH_PARTICLE_RADIUS_M);
}

void LiquidModel::removeGlassware()
{
	isPouring = false;
	delete world;

	world = nullptr;
	liquidParticles = nullptr;
	dashParticles = nullptr;
	iceBodies.clear();
	mintBodies.clear();
	scheduledDrinks.clear();
}

void LiquidModel::empty()
{
	isPouring = false;
	scheduledDrinks.clear();

	if (world)
	{
		// Schedule ice and mint bodies for removal.
		scheduleRemoveIceBodies = true;
		scheduleRemoveMintBodies = true;

		// Schedule particles for removal.
		for (int i = 0; i < liquidParticles->GetParticleCount(); ++i)
		{
			liquidParticles->SetParticleFlags(i, b2_zombieParticle);
		}

		emit liquidEmptied();
	}
}

void LiquidModel::addIce()
{
	scheduleAddIceBody = true;
}

void LiquidModel::addMint()
{
	scheduleAddMintBody = true;
}

void LiquidModel::pour(int ounce, const QString &drinkName)
{
	for (int index = 0; index < ounce; ++index)
	{
		scheduledDrinks.enqueue(drinkName);
	}

	scheduledLiquidPouringElapsedTime = 0;
	pouringElapsedTime = 0;
	isPouring = true;
}

void LiquidModel::dash(const QString &dashName)
{
	currentDash = dashName;
	isDashing = true;
}

void LiquidModel::mix()
{
	isMixing = true;
}

void LiquidModel::update(int deltaTime)
{
	liquidPixmap.fill(Qt::transparent); // Clear pixmap.

	if (world)
	{
		world->Step(1.0f / 60.0f, 8, 3, 3); // Step the Box2D simulation
		this->checkScheduledRemoveIceBodies();
		this->checkScheduledRemoveMintBodies();

		this->checkScheduledAddIceBody();
		this->checkScheduledAddMintBody();

		if (isPouring && !scheduledDrinks.isEmpty())
		{
			if (pouringElapsedTime >= DROPS_INTERVAL_MS)
			{
				pouringElapsedTime = -deltaTime; // Reset.
				this->spawnLiquidParticles();
			}

			pouringElapsedTime += deltaTime;
			scheduledLiquidPouringElapsedTime += deltaTime;
		}

		if (scheduledLiquidPouringElapsedTime >= OUNCE_POURING_DURATION_MS)
		{
			scheduledLiquidPouringElapsedTime = 0;
			scheduledDrinks.dequeue();
		}

		if (isDashing)
		{
			this->spawnDashParticles();
			isDashing = false;
		}

		this->draw();
	}

	if (isMixing)
		isMixing = false;

	emit simulationUpdated(liquidPixmap);
}

void LiquidModel::draw()
{
	// Setup Painter.
	QPainter painter(&liquidPixmap);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

	b2Vec2 *posBuffer;

	// Draw dash particles
	posBuffer = dashParticles->GetPositionBuffer();
	for (int i = 0; i < dashParticles->GetParticleCount(); ++i)
	{
		b2Vec2 particlePosition = posBuffer[i];
		QColor color = dashColors[currentDash];
		painter.setPen(color.darker(125));
		painter.setBrush(color);
		painter.drawEllipse(QPointF(particlePosition.x / CONVERSION_FACTOR, PIXMAP_HEIGHT - (particlePosition.y / CONVERSION_FACTOR)), 1.5, 1.5);
	}

	// Draw liquid particles
	posBuffer = liquidParticles->GetPositionBuffer();
	b2ParticleColor *colorBuffer = liquidParticles->GetColorBuffer();

	for (int i = 0; i < liquidParticles->GetParticleCount(); ++i)
	{
		if (isMixing)
			liquidParticles->SetParticleFlags(i, b2_elasticParticle | b2_colorMixingParticle);

		b2Vec2 particlePosition = posBuffer[i];
		painter.setPen(Qt::NoPen);
		painter.setBrush(QColor(colorBuffer[i].r, colorBuffer[i].g, colorBuffer[i].b, colorBuffer[i].a));
		painter.drawEllipse(QPointF(particlePosition.x / CONVERSION_FACTOR,
									PIXMAP_HEIGHT - (particlePosition.y / CONVERSION_FACTOR)),
							LIQUID_PARTICLE_RADIUS_M / CONVERSION_FACTOR + 1.0f, LIQUID_PARTICLE_RADIUS_M / CONVERSION_FACTOR + 1.0f);
	}

	// Draw mint bodies.
	for (b2Body *mintBody : mintBodies)
	{
		b2Vec2 bodyPos = mintBody->GetPosition();

		int x = (bodyPos.x / CONVERSION_FACTOR) - (mintTexture->size().width() / 2);
		int y = PIXMAP_HEIGHT - ((bodyPos.y / CONVERSION_FACTOR) + (mintTexture->size().height() / 2));

		painter.drawPixmap(x, y, *mintTexture);
	}

	// Draw ice bodies.
	for (b2Body *iceBody : iceBodies)
	{
		b2Vec2 bodyPos = iceBody->GetPosition();

		int x = (bodyPos.x / CONVERSION_FACTOR) - (iceTexture->size().width() / 2);
		int y = PIXMAP_HEIGHT - ((bodyPos.y / CONVERSION_FACTOR) + (iceTexture->size().height() / 2));

		painter.drawPixmap(x, y, *iceTexture);
	}

	painter.end();
}

void LiquidModel::spawnLiquidParticles()
{
	b2Vec2 particlePos;

	for (int j = 0; j < LIQUID_PARTICLES_NUM_SPAWN_VERTICAL; ++j)
	{
		int a = j % 2; // Randomise x coordinate.

		// Def
		b2ParticleDef pdef;
		pdef.color = drinkColors[scheduledDrinks.head()];
		pdef.flags = b2_waterParticle;
		pdef.velocity = {0.0f, DROP_VELOCITY};

		// Y coordinate.
		//particlePos.y = pouringSource.y() - (numParticlesY / 2.0f) * particleSpacingY + j * particleSpacingY;
		particlePos.y = (pouringSource.y - (LIQUID_PARTICLE_RADIUS_M * j)) - LIQUID_PARTICLE_RADIUS_M * 2;

		// Left Particle
		particlePos.x = (pouringSource.x - LIQUID_PARTICLE_RADIUS_M) - ((LIQUID_PARTICLE_RADIUS_M / 2) * a);
		pdef.position = particlePos; // Particle position its spawned at.
		liquidParticles->CreateParticle(pdef); // Spawn

		// Right Particle
		particlePos.x = (pouringSource.x + LIQUID_PARTICLE_RADIUS_M) - ((LIQUID_PARTICLE_RADIUS_M / 2) * a);
		pdef.position = particlePos; // Particle position its spawned at.
		liquidParticles->CreateParticle(pdef); // Spawn.
	}
}

void LiquidModel::spawnDashParticles()
{
	b2Vec2 particlePos;

	for (int j = 0; j < 8; ++j)
	{
		int a = j % 2; // Randomise x coordinate.

		b2ParticleDef pdef;
		pdef.flags = b2_powderParticle;
		pdef.velocity = {0.0f, DROP_VELOCITY};

		// Y coordinate.
		particlePos.y = (pouringSource.y - (DASH_PARTICLE_RADIUS_M * j)) - DASH_PARTICLE_RADIUS_M;

		// Left Particle
		particlePos.x = (pouringSource.x - DASH_PARTICLE_RADIUS_M) - ((DASH_PARTICLE_RADIUS_M / 2) * a);
		pdef.position = particlePos; // Particle position its spawned at.
		dashParticles->SetParticleLifetime(dashParticles->CreateParticle(pdef), 0.8f); // Spawn.

		// Right Particle
		particlePos.x = (pouringSource.x + DASH_PARTICLE_RADIUS_M) - ((DASH_PARTICLE_RADIUS_M / 2) * a);
		pdef.position = particlePos; // Particle position its spawned at.
		dashParticles->SetParticleLifetime(dashParticles->CreateParticle(pdef), 0.8f); // Spawn.
	}
}

void LiquidModel::checkScheduledAddIceBody()
{
	if (scheduleAddIceBody)
	{
		// Random x coordinate within the defined range.
		int xPixels = QRandomGenerator::global()->bounded((int)(bodiesHorizontalSpawnRange[0] / CONVERSION_FACTOR), (int)(bodiesHorizontalSpawnRange[1] / CONVERSION_FACTOR));

		b2BodyDef bdef;
		bdef.type = b2_dynamicBody;
		bdef.position.Set(xPixels * CONVERSION_FACTOR, pouringSource.y);

		b2Body *iceBody = world->CreateBody(&bdef);
		b2CircleShape circle;
		circle.m_radius = ICE_RADIUS_M;
		iceBody->CreateFixture(&circle, ICE_DENSITY);
		iceBody->SetLinearVelocity({0.0f, DROP_VELOCITY});

		iceBodies.push_back(iceBody);

		scheduleAddIceBody = false;
	}
}

void LiquidModel::checkScheduledRemoveIceBodies()
{
	if (scheduleRemoveIceBodies)
	{
		for (b2Body *iceBody : iceBodies)
			world->DestroyBody(iceBody);

		iceBodies.clear();

		scheduleRemoveIceBodies = false;
	}
}

void LiquidModel::checkScheduledAddMintBody()
{
	if (scheduleAddMintBody)
	{
		// Random x coordinate within the defined range.
		int xPixels = QRandomGenerator::global()->bounded((int)(bodiesHorizontalSpawnRange[0] / CONVERSION_FACTOR), (int)(bodiesHorizontalSpawnRange[1] / CONVERSION_FACTOR));

		b2BodyDef bdef;
		bdef.type = b2_dynamicBody;
		bdef.position.Set(xPixels * CONVERSION_FACTOR, pouringSource.y);

		b2Body *mintBody = world->CreateBody(&bdef);
		b2CircleShape circle;
		circle.m_radius = MINT_RADIUS_M;
		mintBody->CreateFixture(&circle, MINT_DENSITY);
		mintBody->SetLinearVelocity({0.0f, DROP_VELOCITY});

		mintBodies.push_back(mintBody);

		scheduleAddMintBody = false;
	}
}

void LiquidModel::checkScheduledRemoveMintBodies()
{
	if (scheduleRemoveMintBodies)
	{
		for (b2Body *mintBody : mintBodies)
			world->DestroyBody(mintBody);

		mintBodies.clear();

		scheduleRemoveMintBodies = false;
	}
}
