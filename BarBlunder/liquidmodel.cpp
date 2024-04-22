#include "liquidmodel.h"
#include "drinkview_size.h"

#include "glassware.h"

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <QDebug>

#include <Box2D/Box2D.h>

LiquidModel::LiquidModel(QWidget *parent)
	: QObject{parent}
	, isSimulationPaused{false}
	, isDrinkEmpty{false}

	, currentDrink{""}

	, pouringSource{0.0f, 0.0f}

	, liquidPixmap{DRINKVIEW_WIDTH, DRINKVIEW_HEIGHT}

	, isMixing{false}
	, collisionBody{nullptr}
{
	iceTexture = new QPixmap(":/images/glasses_liquid/icesphere_16.png");
	b2Vec2 gravity(0.0f, -9.8f * GRAVITY_SCALE);
	world = new b2World(gravity);

	uchar ALPHA = 200;

	// setup colors for each drink
	drinkColors["whiskey"] =		{165, 113, 10, ALPHA};				// Whiskey color
	drinkColors["tequila"] =		{172, 73, 25, ALPHA};				// Tequila color
	drinkColors["rum"] =			{236, 233, 226, ALPHA};				// Rum color
	drinkColors["vodka"] =			{236, 233, 226, ALPHA};				// Vodka color
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
	b2ParticleSystemDef particleSystemDef;
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
	particleSystemDef.viscousStrength = 0.25f; // 1.0f

	// Create the liquid particle system
	liquidParticles = world->CreateParticleSystem(&particleSystemDef);
	liquidParticles->SetRadius(PARTICLE_RADIUS);
	liquidParticles->SetDensity(1.2f);
	//liquidParticles->SetGravityScale(0.4f);
}

LiquidModel::~LiquidModel()
{
	delete iceTexture;
	delete world;
}

void LiquidModel::updateCollisionLayout(const Glassware &glassware)
{
	this->removeCollisionLayout();

	pouringSource.setX(glassware.getPhysicsPouringSource().x());
	pouringSource.setY(DRINKVIEW_HEIGHT - glassware.getPhysicsPouringSource().y());

	const QList<QPointF> &qVs = glassware.getPhysicsCollisionVertices();

	b2Vec2 vs[6];
	vs[0].Set(qVs[0].x(), DRINKVIEW_HEIGHT - qVs[0].y());
	vs[1].Set(qVs[1].x(), DRINKVIEW_HEIGHT - qVs[1].y());
	vs[2].Set(qVs[2].x(), DRINKVIEW_HEIGHT - qVs[2].y());
	vs[3].Set(qVs[3].x(), DRINKVIEW_HEIGHT - qVs[3].y());

	// To create a loop:
	vs[4].Set(qVs[2].x(), DRINKVIEW_HEIGHT - qVs[2].y());
	vs[5].Set(qVs[1].x(), DRINKVIEW_HEIGHT - qVs[1].y());

	b2BodyDef collisionBodyDef;
	collisionBodyDef.type = b2_kinematicBody;
	collisionBody = world->CreateBody(&collisionBodyDef);

	b2ChainShape collisionShape;
	collisionShape.CreateLoop(vs, 6);
	collisionBody->CreateFixture(&collisionShape, 0.0f);
}

void LiquidModel::removeCollisionLayout()
{
	if (collisionBody)
		world->DestroyBody(collisionBody);
}

void LiquidModel::addIce()
{
	b2BodyDef bdef;
	bdef.type = b2_dynamicBody;
	bdef.position.Set(pouringSource.x(), pouringSource.y());

	b2Body *iceBody = world->CreateBody(&bdef);
	b2CircleShape circle;
	circle.m_radius = 8;
	iceBody->CreateFixture(&circle, 3.0f);
	iceBody->SetLinearVelocity({0.0f, -9.8f * GRAVITY_SCALE});

	iceBodies.push_back(iceBody);
}

void LiquidModel::addLiquid(int volume)
{
	if (isDrinkEmpty)
		isDrinkEmpty = false;

	int numParticlesY = 4; // number of particles spawned vertically

	QTimer* timer = new QTimer(); // Allocate QTimer dynamically
	connect(timer, &QTimer::timeout, this, [=]() {

		if (isSimulationPaused)
			return;

		// Spawn particles for 3 seconds
		static float elapsedTime = 0.0f;
		b2Vec2 particlePos;

		// attempting to use volume
		if (elapsedTime < volume && !isDrinkEmpty)
		{
			// Spawn particles.
			for (int j = 0; j < numParticlesY; ++j)
			{
				int a = j % 2;

				// Def
				b2ParticleDef pdef;
				pdef.color = drinkColors[currentDrink];
				pdef.flags = b2_elasticParticle;
				pdef.velocity = {0.0f, -5.9f * GRAVITY_SCALE};

				// Y coordinate.
				//particlePos.y = pouringSource.y() - (numParticlesY / 2.0f) * particleSpacingY + j * particleSpacingY;
				particlePos.y = (pouringSource.y() - (PARTICLE_RADIUS * j)) - PARTICLE_RADIUS * 2;

				// Left Particle
				particlePos.x = (pouringSource.x() - PARTICLE_RADIUS) - ((PARTICLE_RADIUS / 2) * a);
				pdef.position = particlePos; // Particle position its spawned at.
				liquidParticles->CreateParticle(pdef); // Spawn

				// Right Particle
				particlePos.x = (pouringSource.x() + PARTICLE_RADIUS) - ((PARTICLE_RADIUS / 2) * a);
				pdef.position = particlePos; // Particle position its spawned at.
				liquidParticles->CreateParticle(pdef); // Spawn.
			}

			elapsedTime += 0.1; // Increase elapsed time
		}
		else
		{
			timer->stop(); // Stop the timer after 3 seconds
			delete timer; // delete timer
			elapsedTime = 0.0;
		}
	});

	timer->start(100);
}

void LiquidModel::updateDrinkColor(const QString &drinkName)
{
	currentDrink = drinkName;
}

void LiquidModel::clear()
{
	// Destroy ice bodies.
	for (b2Body *iceBody : iceBodies)
	{
		world->DestroyBody(iceBody);
	}

	iceBodies.clear();

	// Destroy liquid particle system.
	if (liquidParticles)
	{
		int particleCount = liquidParticles->GetParticleCount();

		for (int i = 0; i < particleCount; ++i)
		{
			liquidParticles->DestroyParticle(i, true);
		}
	}

	isDrinkEmpty = true;

	emit liquidEmptied();
}

void LiquidModel::setIsSimulationPaused(bool state)
{
	isSimulationPaused = state;
}

void LiquidModel::stir()
{
	//liquidParticles->ApplyForce(0, liquidParticles->GetParticleCount() - 1, {0.0f, 5.9f * GRAVITY_SCALE});
	//collisionBody->SetAngularVelocity(1.74533f);
	//collisionBody->SetTransform(collisionBody->GetPosition(), 0.0872665f);
	collisionBody->SetTransform(collisionBody->GetPosition(), 0.0f);
	isMixing = true;
}

void LiquidModel::update()
{
	if (isSimulationPaused)
		return;

	// Step the Box2D simulation
	world->Step(1.0f / 60.0f, 8, 3);



	this->draw();

	if (isMixing)
		isMixing = false;

	emit simulationUpdated(liquidPixmap);
}

void LiquidModel::draw()
{
	//Set up Pixmap
	liquidPixmap.fill(Qt::transparent);
	QPainter painter(&liquidPixmap);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);// CompositionMode_SourceOver

	// Draw liquid particles
	b2Vec2 *posBuffer = liquidParticles->GetPositionBuffer();
	b2ParticleColor *colorBuffer = liquidParticles->GetColorBuffer();

	for (int i = 0; i < liquidParticles->GetParticleCount(); ++i)
	{
		if (isMixing)
			liquidParticles->SetParticleFlags(i, b2_elasticParticle | b2_colorMixingParticle);

		// Get particle position
		b2Vec2 particlePosition = posBuffer[i];
		painter.setPen(Qt::NoPen);
		painter.setBrush(QColor(colorBuffer[i].r, colorBuffer[i].g, colorBuffer[i].b, colorBuffer[i].a));
		painter.drawEllipse(QPointF(particlePosition.x, DRINKVIEW_HEIGHT - particlePosition.y), 5, 5); // Adjust the size as needed.
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
