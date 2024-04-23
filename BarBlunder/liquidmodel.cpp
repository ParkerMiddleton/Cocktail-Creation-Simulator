#include "liquidmodel.h"
#include "drinkview_size.h"

#include "glassware.h"

#include <QWidget>
#include <QPainter>
#include <QPen>

#include <Box2D/Box2D.h>

LiquidModel::LiquidModel(QWidget *parent)
	: QObject{parent}

	, isMixing{false}
	, isPouring{false}
	, isDashing{false}
	, pouringElapsedTime{0}
	, scheduledLiquidPouringElapsedTime{0}

	, liquidPixmap{DRINKVIEW_WIDTH, DRINKVIEW_HEIGHT}
	, iceTexture{nullptr}

	, scheduleAddIceBody{false}
	, scheduleRemoveIceBodies{false}

	, gravity{0.0f, -9.8f}
	, pouringSource{0.0f, 0.0f}

	, world{nullptr}
	, liquidParticles{nullptr}
	, dashParticles{nullptr}
{
	QPixmap image(":/images/maindrink/icesphere.png");
	iceTexture = new QPixmap(image.scaled((ICE_RADIUS_M / FROM_PIXEL_FACTOR) * 2, (ICE_RADIUS_M / FROM_PIXEL_FACTOR) * 2, Qt::KeepAspectRatio, Qt::SmoothTransformation));

	uchar ALPHA = 200;

	// setup colors for each drink
	drinkColors["whiskey"] =		{165, 113, 10, ALPHA};				// Whiskey color
	drinkColors["tequila"] =		{172, 73, 25, ALPHA};				// Tequila color
	drinkColors["rum"] =			{236, 233, 226, ALPHA};				// Rum color
	drinkColors["vodka"] =			{236, 233, 226, 50};				// Vodka color
	drinkColors["gin"] =			{236, 233, 226, ALPHA};				// Gin color
	drinkColors["kahlua"] =			{28, 1, 2, ALPHA};					// kahlua
	drinkColors["burbon"] =			{165, 113, 10, ALPHA};				// burbon
	drinkColors["tripe sec"] =		{236, 233, 226, ALPHA};				// triple sec
    drinkColors["sweet and sour mix"] =	{210, 196, 112, ALPHA};				// sweet n sour
	drinkColors["grenadine"] =		{180, 19, 41, ALPHA};				// grenadine
	drinkColors["simple syrup"] =	{236, 233, 226, ALPHA};				// simpe syrup
	drinkColors["agave nectar"] =	{245, 183, 10, ALPHA};				// agave nectar
	drinkColors["sprite"] =			{236, 233, 226, ALPHA};				// sprite
	drinkColors["coke"] =			{56, 45, 43, ALPHA};				// coke
	drinkColors["half n half"] =	{255, 255, 255, ALPHA};				// half n half
	drinkColors["lime juice"] =		{158, 180, 80, ALPHA};				// lime juice
	drinkColors["ginger beer"] =	{253, 237, 115, ALPHA};				// ginger beer
	drinkColors["olive juice"] =	{207, 178, 112, ALPHA};				// olive juice
	drinkColors["bitters"] =		{179, 102, 110, ALPHA};				// bitters

	// Setup liquid particle system.
	liquidParticlesDef.colorMixingStrength = 0.25f;
	liquidParticlesDef.destroyByAge = true;

	liquidParticlesDef.dampingStrength = 0.2f;
	liquidParticlesDef.surfaceTensionPressureStrength = 0.1f;
	liquidParticlesDef.surfaceTensionNormalStrength = 1.0f;
	liquidParticlesDef.springStrength = 0.0f;
	liquidParticlesDef.elasticStrength = 0.1f;
	liquidParticlesDef.staticPressureStrength = 0.2f;
	liquidParticlesDef.powderStrength = 0.0f;
	liquidParticlesDef.viscousStrength = 1.0f;

	liquidParticlesDef.radius = LIQUID_PARTICLE_RADIUS_M;
	liquidParticlesDef.gravityScale = 0.4f;
	//liquidParticlesDef.density = 1.2f;

	// Setup dash particle system.
	dashParticlesDef.destroyByAge = true;
	dashParticlesDef.radius = DASH_PARTICLE_RADIUS_M;
	dashParticlesDef.density = 1.2f;

	//dashParticlesDef.dampingStrength = 0.2f;
}

LiquidModel::~LiquidModel()
{
	delete iceTexture;

	this->removeGlassware();
}

void LiquidModel::updateGlassware(const Glassware &glassware)
{
	this->removeGlassware();

	world = new b2World(gravity);

	pouringSource.x = glassware.getPhysicsPouringSource().x() * FROM_PIXEL_FACTOR;
	pouringSource.y = (DRINKVIEW_HEIGHT - glassware.getPhysicsPouringSource().y()) * FROM_PIXEL_FACTOR;

	const QList<QPointF> &qVs = glassware.getPhysicsCollisionVertices();

	b2Vec2 collisionVertices[6];
	collisionVertices[0].Set(qVs[0].x() * FROM_PIXEL_FACTOR, (DRINKVIEW_HEIGHT - qVs[0].y()) * FROM_PIXEL_FACTOR);
	collisionVertices[1].Set(qVs[1].x() * FROM_PIXEL_FACTOR, (DRINKVIEW_HEIGHT - qVs[1].y()) * FROM_PIXEL_FACTOR);
	collisionVertices[2].Set(qVs[2].x() * FROM_PIXEL_FACTOR, (DRINKVIEW_HEIGHT - qVs[2].y()) * FROM_PIXEL_FACTOR);
	collisionVertices[3].Set(qVs[3].x() * FROM_PIXEL_FACTOR, (DRINKVIEW_HEIGHT - qVs[3].y()) * FROM_PIXEL_FACTOR);

	// To create a loop:
	collisionVertices[4].Set(collisionVertices[2].x, collisionVertices[2].y);
	collisionVertices[5].Set(collisionVertices[1].x, collisionVertices[1].y);

	b2BodyDef collisionBodyDef;
	b2Body *collisionBody = world->CreateBody(&collisionBodyDef);

	b2ChainShape collisionShape;
	collisionShape.CreateLoop(collisionVertices, 6);
	collisionBody->CreateFixture(&collisionShape, 0.0f);

	// Create the liquid particle system
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
	scheduledDrinks.clear();
}

void LiquidModel::empty()
{
	isPouring = false;
	scheduledDrinks.clear();

	if (world)
	{
		// Schedule ice bodies for removal.
		scheduleRemoveIceBodies = true;

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

void LiquidModel::dash()
{
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
		world->Step(1.0f / 60.0f, 8, 3); // Step the Box2D simulation
		this->checkSheduledRemoveIceBodies();
		this->checkScheduledAddIceBody();

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

	// Draw liquid particles
	b2Vec2 *posBuffer = liquidParticles->GetPositionBuffer();
	b2ParticleColor *colorBuffer = liquidParticles->GetColorBuffer();

	for (int i = 0; i < liquidParticles->GetParticleCount(); ++i)
	{
		if (isMixing)
			liquidParticles->SetParticleFlags(i, b2_elasticParticle | b2_colorMixingParticle);

		b2Vec2 particlePosition = posBuffer[i];
		painter.setPen(Qt::NoPen);
		painter.setBrush(QColor(colorBuffer[i].r, colorBuffer[i].g, colorBuffer[i].b, colorBuffer[i].a));
		painter.drawEllipse(QPointF(particlePosition.x / FROM_PIXEL_FACTOR,
									DRINKVIEW_HEIGHT - (particlePosition.y / FROM_PIXEL_FACTOR)),
							LIQUID_PARTICLE_RADIUS_M / FROM_PIXEL_FACTOR + 1.0f, LIQUID_PARTICLE_RADIUS_M / FROM_PIXEL_FACTOR + 1.0f);
	}

	// Draw dash particles
	posBuffer = dashParticles->GetPositionBuffer();

	for (int i = 0; i < dashParticles->GetParticleCount(); ++i)
	{
		b2Vec2 particlePosition = posBuffer[i];
		painter.setPen(Qt::NoPen);
		painter.setBrush(QColor(166, 99, 41));
		painter.drawEllipse(QPointF(particlePosition.x / FROM_PIXEL_FACTOR, DRINKVIEW_HEIGHT - (particlePosition.y / FROM_PIXEL_FACTOR)), 5, 5);
	}

	// Draw ice bodies.
	for (b2Body *iceBody : iceBodies)
	{
		b2Vec2 bodyPos = iceBody->GetPosition();

		int x = (bodyPos.x / FROM_PIXEL_FACTOR) - (iceTexture->size().width() / 2);
		int y = DRINKVIEW_HEIGHT - ((bodyPos.y / FROM_PIXEL_FACTOR) + (iceTexture->size().height() / 2));

		painter.drawPixmap(x, y, *iceTexture);
	}

	painter.end();
}

void LiquidModel::spawnLiquidParticles()
{
	b2Vec2 particlePos;

	for (int j = 0; j < LIQUID_PARTICLES_NUM_SPAWN_VERTICAL; ++j)
	{
		int a = j % 2;

		// Def
		b2ParticleDef pdef;
		pdef.color = drinkColors[scheduledDrinks.head()];
		pdef.flags = b2_viscousParticle;
		pdef.velocity = {0.0f, LIQUID_PARTICLE_DROP_VELOCITY};

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

	for (int index = 0; index < 2; index++)
	{
		for (int j = 0; j < 4; ++j)
		{
			int a = j % 2;

			// Def
			b2ParticleDef pdef;
			pdef.flags = b2_elasticParticle;
			pdef.velocity = {0.0f, LIQUID_PARTICLE_DROP_VELOCITY};

			// Y coordinate.
			//particlePos.y = pouringSource.y() - (numParticlesY / 2.0f) * particleSpacingY + j * particleSpacingY;
			particlePos.y = (pouringSource.y - (DASH_PARTICLE_RADIUS_M * j)) - DASH_PARTICLE_RADIUS_M;

			// Left Particle
			particlePos.x = (pouringSource.x - DASH_PARTICLE_RADIUS_M) - ((DASH_PARTICLE_RADIUS_M / 2) * a);
			pdef.position = particlePos; // Particle position its spawned at.
			dashParticles->CreateParticle(pdef); // Spawn

			// Right Particle
			particlePos.x = (pouringSource.x + DASH_PARTICLE_RADIUS_M) - ((DASH_PARTICLE_RADIUS_M / 2) * a);
			pdef.position = particlePos; // Particle position its spawned at.
			dashParticles->CreateParticle(pdef); // Spawn.
		}
	}
}

void LiquidModel::checkScheduledAddIceBody()
{
	if (scheduleAddIceBody)
	{
		b2BodyDef bdef;
		bdef.type = b2_dynamicBody;
		bdef.position.Set(pouringSource.x, pouringSource.y);

		b2Body *iceBody = world->CreateBody(&bdef);
		b2CircleShape circle;
		circle.m_radius = ICE_RADIUS_M;
		iceBody->CreateFixture(&circle, ICE_DENSITY);
		//iceBody->SetLinearVelocity({0.0f, -9.8f * GRAVITY_SCALE});

		iceBodies.push_back(iceBody);

		scheduleAddIceBody = false;
	}
}

void LiquidModel::checkSheduledRemoveIceBodies()
{
	if (scheduleRemoveIceBodies)
	{
		for (b2Body *iceBody : iceBodies)
			world->DestroyBody(iceBody);

		iceBodies.clear();

		scheduleRemoveIceBodies = false;
	}
}
