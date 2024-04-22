#include "liquidmodel.h"
#include "drinkview_size.h"

#include "glassware.h"

#include <QWidget>
#include <QPainter>
#include <QPen>

#include <Box2D/Box2D.h>

LiquidModel::LiquidModel(QWidget *parent)
	: QObject{parent}
	, isStirring{false}
	, isPouring{false}
	, pouringElapsedTime{0}

	, currentDrink{""}

	, liquidPixmap{DRINKVIEW_WIDTH, DRINKVIEW_HEIGHT}
	, iceTexture{nullptr}

	, addIceBody{false}
	, removeIceBodies{false}

	, gravity{0.0f, -9.8f * GRAVITY_SCALE}
	, world{nullptr}
	, pouringSource{0.0f, 0.0f}
	, liquidParticles{nullptr}
{
	iceTexture = new QPixmap(":/images/glasses_liquid/icecube_16.png");

	uchar ALPHA = 200;

	// setup colors for each drink
	drinkColors["whiskey"] =		{165, 113, 10, ALPHA};				// Whiskey color
	drinkColors["tequila"] =		{172, 73, 25, ALPHA};				// Tequila color
	drinkColors["rum"] =			{236, 233, 226, ALPHA};				// Rum color
	drinkColors["vodka"] =			{236, 233, 226, 50};				// Vodka color
	drinkColors["gin"] =			{236, 233, 226, ALPHA};				// Gin color
	drinkColors["kahlua"] =			{28, 1, 2, ALPHA};					//kahlua
	drinkColors["burbon"] =			{165, 113, 10, ALPHA};				//burbon
	drinkColors["tripe sec"] =		{236, 233, 226, ALPHA};				//triple sec
	drinkColors["sweet n sour"] =	{210, 196, 112, ALPHA};				//sweet n sour
	drinkColors["grenadine"] =		{180, 19, 41, ALPHA};				//grenadine
	drinkColors["simple syrup"] =	{236, 233, 226, ALPHA};				//simpe syrup
	drinkColors["agave nectar"] =	{245, 183, 10, ALPHA};				//agave nectar
	drinkColors["sprite"] =			{236, 233, 226, ALPHA};				//sprite
	drinkColors["coke"] =			{56, 45, 43, ALPHA};				//coke
	drinkColors["half n half"] =	{255, 255, 255, ALPHA};				//half n half
	drinkColors["lime juice"] =		{158, 180, 80, ALPHA};				//lime juice
	drinkColors["ginger beer"] =	{253, 237, 115, ALPHA};				//ginger beer
	drinkColors["olive juice"] =	{207, 178, 112, ALPHA};				//olive juice
	drinkColors["bitters"] =		{179, 102, 110, ALPHA};				//bitters

	// Setup particle system.
	particleSystemDef.colorMixingStrength = 0.25f;
	particleSystemDef.destroyByAge = true;

	// Define the liquid particle system parameters (idk how many are neccessary here just was messing around)
	particleSystemDef.dampingStrength = 0.2f; // Particle damping strength
	particleSystemDef.surfaceTensionPressureStrength = 0.1f;
	particleSystemDef.surfaceTensionNormalStrength = 1.0f;
	particleSystemDef.springStrength = 0.0f;
	particleSystemDef.elasticStrength = 0.1f;
	particleSystemDef.staticPressureStrength = 0.2f;
	particleSystemDef.powderStrength = 0.0f;
	particleSystemDef.viscousStrength = 1.0f;

	particleSystemDef.radius = PARTICLE_RADIUS;
	particleSystemDef.density = 1.2f;
	//particleSystemDef.gravityScale = 0.4f;
}

LiquidModel::~LiquidModel()
{
	delete iceTexture;

	this->removeGlassware();
	delete world;
}

void LiquidModel::updateGlassware(const Glassware &glassware)
{
	this->removeGlassware();

	world = new b2World(gravity);

	pouringSource.x = glassware.getPhysicsPouringSource().x();
	pouringSource.y = DRINKVIEW_HEIGHT - glassware.getPhysicsPouringSource().y();

	const QList<QPointF> &qVs = glassware.getPhysicsCollisionVertices();

	b2Vec2 collisionVertices[6];
	collisionVertices[0].Set(qVs[0].x(), DRINKVIEW_HEIGHT - qVs[0].y());
	collisionVertices[1].Set(qVs[1].x(), DRINKVIEW_HEIGHT - qVs[1].y());
	collisionVertices[2].Set(qVs[2].x(), DRINKVIEW_HEIGHT - qVs[2].y());
	collisionVertices[3].Set(qVs[3].x(), DRINKVIEW_HEIGHT - qVs[3].y());

	// To create a loop:
	collisionVertices[4].Set(qVs[2].x(), DRINKVIEW_HEIGHT - qVs[2].y());
	collisionVertices[5].Set(qVs[1].x(), DRINKVIEW_HEIGHT - qVs[1].y());

	b2BodyDef collisionBodyDef;
	b2Body *collisionBody = world->CreateBody(&collisionBodyDef);

	b2ChainShape collisionShape;
	collisionShape.CreateLoop(collisionVertices, 6);
	collisionBody->CreateFixture(&collisionShape, 0.0f);

	// Create the liquid particle system
	liquidParticles = world->CreateParticleSystem(&particleSystemDef);
}

void LiquidModel::removeGlassware()
{
	isPouring = false;
	delete world;

	world = nullptr;
	liquidParticles = nullptr;
	iceBodies.clear();
}

void LiquidModel::empty()
{
	if (!world)
		return;

	// Destroy ice bodies.
	removeIceBodies = true;

	// Destroy liquid particle system.
	for (int i = 0; i < liquidParticles->GetParticleCount(); ++i)
	{
		liquidParticles->SetParticleFlags(i, b2_zombieParticle);
	}

	emit liquidEmptied();
}

void LiquidModel::addIce()
{
	addIceBody = true;
}

void LiquidModel::updateDrinkColor(const QString &drinkName)
{
	currentDrink = drinkName;
}


void LiquidModel::dashPour(int ounce)
{

}

void LiquidModel::mix()
{
	isStirring = true;
}


void LiquidModel::startPouring()
{
	pouringElapsedTime = 0;
	isPouring = true;
}

void LiquidModel::stopPouring()
{
	isPouring = false;
}

void LiquidModel::update(int deltaTime)
{
	liquidPixmap.fill(Qt::transparent); // Clear pixmap.

	if (world)
	{
		world->Step(1.0f / 60.0f, 8, 3); // Step the Box2D simulation

		// Destroy scheduled ice bodies.
		if (removeIceBodies)
		{
			this->destroyIceBodies();
			removeIceBodies = false;
		}

		// Create scheduled ice bodies.
		if (addIceBody)
		{
			this->createIceBody();
			addIceBody = false;
		}

		if (isPouring)
		{
			// attempting to use volume
			if (pouringElapsedTime >= 80)
			{
				pouringElapsedTime = -deltaTime; // Reset.
				this->spawnParticles();
			}

			pouringElapsedTime += deltaTime;
		}

		this->draw();
	}

	if (isStirring)
		isStirring = false;

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
		if (isStirring)
			liquidParticles->SetParticleFlags(i, b2_elasticParticle | b2_colorMixingParticle);

		b2Vec2 particlePosition = posBuffer[i];
		painter.setPen(Qt::NoPen);
		painter.setBrush(QColor(colorBuffer[i].r, colorBuffer[i].g, colorBuffer[i].b, colorBuffer[i].a));
		painter.drawEllipse(QPointF(particlePosition.x, DRINKVIEW_HEIGHT - particlePosition.y), 5, 5);
	}

	// Draw ice spheres.
	for (b2Body *iceBody : iceBodies)
	{
		b2Vec2 bodyPos = iceBody->GetPosition();

		int x = bodyPos.x - (iceTexture->size().width() / 2);
		int y = DRINKVIEW_HEIGHT - (bodyPos.y + (iceTexture->size().height() / 2));

		painter.drawPixmap(x, y, *iceTexture);
	}

	painter.end();
}

void LiquidModel::spawnParticles()
{
	b2Vec2 particlePos;

	for (int j = 0; j < PARTICLES_NUM_SPAWN_VERTICAL; ++j)
	{
		int a = j % 2;

		// Def
		b2ParticleDef pdef;
		pdef.color = drinkColors[currentDrink];
		pdef.flags = b2_elasticParticle;
		pdef.velocity = {0.0f, -4.9f * GRAVITY_SCALE};

		// Y coordinate.
		//particlePos.y = pouringSource.y() - (numParticlesY / 2.0f) * particleSpacingY + j * particleSpacingY;
		particlePos.y = (pouringSource.y - (PARTICLE_RADIUS * j)) - PARTICLE_RADIUS * 2;

		// Left Particle
		particlePos.x = (pouringSource.x - PARTICLE_RADIUS) - ((PARTICLE_RADIUS / 2) * a);
		pdef.position = particlePos; // Particle position its spawned at.
		liquidParticles->CreateParticle(pdef); // Spawn

		// Right Particle
		particlePos.x = (pouringSource.x + PARTICLE_RADIUS) - ((PARTICLE_RADIUS / 2) * a);
		pdef.position = particlePos; // Particle position its spawned at.
		liquidParticles->CreateParticle(pdef); // Spawn.
	}
}

void LiquidModel::createIceBody()
{
	b2BodyDef bdef;
	bdef.type = b2_dynamicBody;
	bdef.position.Set(pouringSource.x, pouringSource.y);

	b2Body *iceBody = world->CreateBody(&bdef);
	b2CircleShape circle;
	circle.m_radius = 8;
	iceBody->CreateFixture(&circle, 3.0f);
	//iceBody->SetLinearVelocity({0.0f, -9.8f * GRAVITY_SCALE});

	iceBodies.push_back(iceBody);
}

void LiquidModel::destroyIceBodies()
{
	for (b2Body *iceBody : iceBodies)
		world->DestroyBody(iceBody);

	iceBodies.clear();
}
