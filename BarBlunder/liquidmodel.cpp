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
	, liquidColor{0, 0, 0, 0}

	, pouringSource{0.0f, 0.0f}

	, liquidPixmap{DRINKVIEW_WIDTH, DRINKVIEW_HEIGHT}

	, collisionBody{nullptr}
{
	iceTexture = new QPixmap(":/images/glasses_liquid/icesphere_16.png");
	b2Vec2 gravity(0.0f, -38.81f);
	world = new b2World(gravity);

	// setup timer
	simulationUpdateTimer = new QTimer(this);
	connect(simulationUpdateTimer, &QTimer::timeout, this, &LiquidModel::updateSimulation);
	simulationUpdateTimer->start(16); // Update every 16 milliseconds (approximately 60 FPS)

	// setup colors for each drink
	drinkColors["whiskey"] =		QColor(165, 113, 10);        // Whiskey color
	drinkColors["tequila"] =		QColor(172, 73, 25);        // Tequila color
	drinkColors["rum"] =			QColor(236, 233, 226);            // Rum color
	drinkColors["vodka"] =			QColor(236, 233, 226);        // Vodka color
	drinkColors["gin"] =			QColor(236, 233, 226);            // Gin color
	drinkColors["kahlua"] =			QColor(28, 1, 2);            //kahlua
	drinkColors["burbon"] =			QColor(165, 113, 10);        //burbon
	drinkColors["tripe sec"] =		QColor(236, 233, 226);     //triple sec
	drinkColors["sweet n sour"] =	QColor(210, 196, 112);  //sweet n sour
	drinkColors["grenadine"] =		QColor(180, 19, 41);     //grenadine
	drinkColors["simple syrup"] =	QColor(236, 233, 226);  //simpe syrup
	drinkColors["agave nectar"] =	QColor(245, 183, 10);  //agave nectar
	drinkColors["sprite"] =			QColor(236, 233, 226);        //sprite
	drinkColors["coke"] =			QColor(56, 45, 43);          //coke
	drinkColors["half n half"] =	QColor(255, 255, 255);   //half n half
	drinkColors["lime juice"] =		QColor(158, 180, 80);    //lime juice
	drinkColors["ginger beer"] =	QColor(253, 237, 115);   //ginger beer
	drinkColors["olive juice"] =	QColor(207, 178, 112);   //olive juice
	drinkColors["bitters"] =		QColor(179, 102, 110);       //bitters

	// Setup particle system.
	b2ParticleSystemDef particleSystemDef;
	// Define the liquid particle system parameters (idk how many are neccessary here just was messing around)
	particleSystemDef.gravityScale = -9.0f;
	particleSystemDef.radius = 3.75f; // Particle radius
	particleSystemDef.dampingStrength = 0.2f; // Particle damping strength
	particleSystemDef.gravityScale = 1.0f; // Particle gravity scale
	particleSystemDef.viscousStrength = 0.25f; // Particle viscous strength
	particleSystemDef.surfaceTensionPressureStrength = 0.1f; // Particle surface tension pressure strength
	particleSystemDef.surfaceTensionNormalStrength = 1.0f; // Particle surface tension normal strength
	particleSystemDef.springStrength = 0.0f; // Particle spring strength
	particleSystemDef.elasticStrength = 0.1f; // Particle elastic strength
	particleSystemDef.staticPressureStrength = 0.2f; // Particle dynamic pressure strength
	particleSystemDef.staticPressureStrength = 0.2f; // Particle static pressure strength
	particleSystemDef.powderStrength = 0.0f; // Particle powder strength
	particleSystemDef.viscousStrength = 1.0f; // Particle viscous strength
	particleSystemDef.colorMixingStrength = 1.0f; // Particle color mixing strength
	particleSystemDef.destroyByAge = true; // Destroy particles as they age
	particleSystemDef.density = 1.0f;

	// Create the liquid particle system
	liquidParticles = world->CreateParticleSystem(&particleSystemDef);
	liquidParticles->SetGravityScale(9.0f);
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
	b2CircleShape circle;
	circle.m_radius = 8;

	b2BodyDef bdef;

	bdef.type = b2_dynamicBody;
	bdef.position.Set(pouringSource.x(), pouringSource.y());

	b2Body *iceBody = world->CreateBody(&bdef);
	iceBody->CreateFixture(&circle, 15);

	iceBodies.push_back(iceBody);
}

void LiquidModel::addLiquid(int volume)
{
	if (isDrinkEmpty)
		isDrinkEmpty = false;

	int numParticlesX = 3; // number of particles spawned horizontally
	int numParticlesY = 3; // number of particles spawned vertically
	float particleSpacingX = 4.5f; // Adjust the spacing between particles in the x direction
	float particleSpacingY = 4.5f; // Adjust the spacing between particles in the y direction

	liquidColor = this->blendColorAlpha(liquidColor, drinkColors.value(currentDrink));

	QTimer* timer = new QTimer(); // Allocate QTimer dynamically
	connect(timer, &QTimer::timeout, this, [=]() {

		if (isSimulationPaused)
			return;
		// Spawn particles for 3 seconds
		static float elapsedTime = 0.0f;

		// attempting to use volume
		if (elapsedTime < volume && !isDrinkEmpty)
		{
			// Spawn particles in the loop
			for (int i = 0; i < numParticlesX; ++i)
			{
				for (int j = 0; j < numParticlesY; ++j)
				{
					b2Vec2 particlePosition;
					particlePosition.x = pouringSource.x() - (numParticlesX / 2.0f) * particleSpacingX + i * particleSpacingX;
					particlePosition.y = pouringSource.y() - (numParticlesY / 2.0f) * particleSpacingY + j * particleSpacingY;

					// Create a particle
					b2ParticleDef particleDef;
					particleDef.flags = b2_elasticParticle;
					particleDef.position = particlePosition; // Particle position its spawned at

					//get current color
					// convert to a b2color to be used with particleDef
					b2ParticleColor b2c;
					b2c.Set(liquidColor.red(), liquidColor.green(), liquidColor.blue(), liquidColor.alpha());

					particleDef.color = b2c;

					// Spawn the particle
					liquidParticles->CreateParticle(particleDef);
				}
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
	liquidColor = {0, 0, 0, 0};

	emit liquidEmptied();
}

void LiquidModel::setIsSimulationPaused(bool state)
{
	isSimulationPaused = state;
}

void LiquidModel::updateSimulation()
{
	if (isSimulationPaused)
		return;

	// Step the Box2D simulation
	world->Step(1.0f / 60.0f, 1, 3);

	// Draw ice spheres and liquid.

	//Set up Pixmap
	liquidPixmap.fill(Qt::transparent);
	QPainter painter(&liquidPixmap);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

	// Draw ice spheres.
	for (b2Body *iceBody : iceBodies)
	{
		b2Vec2 bodyPos = iceBody->GetPosition();

		int x = bodyPos.x - (iceTexture->size().width() / 2);
		int y = DRINKVIEW_HEIGHT - (bodyPos.y + (iceTexture->size().height() / 2));

		painter.drawPixmap(x, y, *iceTexture);
	}

	// Draw liquid particles
	for (int i = 0; i < liquidParticles->GetParticleCount(); ++i)
	{
		// Get particle position
		b2Vec2 particlePosition = liquidParticles->GetPositionBuffer()[i];
		painter.setPen(Qt::NoPen);
		painter.setBrush(liquidColor);
		painter.drawEllipse(QPointF(particlePosition.x, DRINKVIEW_HEIGHT - particlePosition.y), 4, 4); // Adjust the size as needed
	}

	painter.end();

	emit simulationUpdated(liquidPixmap);
}

QColor LiquidModel::blendColorAlpha(QColor fgc, QColor bgc)
{
	// Taken from: https://stackoverflow.com/a/727339
	// Extremely suboptimal implementation.

	if (fgc.alpha() > 0)
		fgc.setAlpha(150);

	if (bgc.alpha() > 0)
		bgc.setAlpha(150);

	// Normalize.
	struct DClr { double r = 0.0; double g = 0.0; double b = 0.0; double a = 0.0; };
	DClr result;
	DClr bg{(double)bgc.red() / 255.0, (double)bgc.green() / 255.0, (double)bgc.blue() / 255.0, (double)bgc.alpha() / 255.0};
	DClr fg{(double)fgc.red() / 255.0, (double)fgc.green() / 255.0, (double)fgc.blue() / 255.0, (double)fgc.alpha() / 255.0};

	//

	result.a = 1 - (1 - fg.a) * (1 - bg.a);

	if (result.a >= 1.0e-6) // If not fully transparent
	{
		result.r = fg.r * fg.a / result.a + bg.r * bg.a * (1 - fg.a) / result.a;
		result.g = fg.g * fg.a / result.a + bg.g * bg.a * (1 - fg.a) / result.a;
		result.b = fg.b * fg.a / result.a + bg.b * bg.a * (1 - fg.a) / result.a;
	}

	return QColor{uchar(255.0 * result.r), uchar(255.0 * result.g), uchar(255.0 * result.b), 150};
}
