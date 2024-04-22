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
	, isDrinkEmpty{false}
	, volume{0}
	, isSimulationPaused{false}

	, liquidColor{0, 0, 0, 0}

	, liquidPixmap{DRINKVIEW_WIDTH, DRINKVIEW_HEIGHT}
	, blankPixmap{DRINKVIEW_WIDTH, DRINKVIEW_HEIGHT}
	, pouringSource{0.0f, 0.0f}
	, iceDropSource{130.0f, 70.0f}

	, collisionBody{nullptr}
{
	iceTexture = new QPixmap(":/images/glasses_liquid/icesphere_16.png");
	b2Vec2 gravity(0.0f, -38.81f);
	world = new b2World(gravity);

	// setup timer
	updateTimer = new QTimer(this);
	connect(updateTimer, &QTimer::timeout, this, &LiquidModel::updateSimulation);
	updateTimer->start(16); // Update every 16 milliseconds (approximately 60 FPS)

	// setup colors for each drink
	drinkColors["whiskey"] = QColor(165, 113, 10, 150);		// Whiskey color
	drinkColors["tequila"] = QColor(172, 73, 25, 150);		// Tequila color
	drinkColors["rum"] = QColor(236, 233, 226, 150);			// Rum color
	drinkColors["vodka"] = QColor(236, 233, 226, 150);		// Vodka color
	drinkColors["gin"] = QColor(236, 233, 226, 150);			// Gin color
	drinkColors["kahlua"] = QColor(28, 1, 2, 150);			//kahlua
	drinkColors["burbon"] = QColor(165, 113, 10, 150);        //burbon
	drinkColors["tripe sec"] = QColor(255,255,255,150);     //triple sec
	drinkColors["sweet n sour"] = QColor(255,255,255,150);  //sweet n sour
	drinkColors["grenadine"] = QColor(255,255,255,150);     //grenadine
	drinkColors["simple syrup"] = QColor(255,255,255,150);  //simpe syrup
	drinkColors["agave nectar"] = QColor(255,255,255,150);  //agave nectar
	drinkColors["sprite"] = QColor(255,255,255,150);        //sprite
	drinkColors["coke"] = QColor(255,255,255,150);          //coke
	drinkColors["half n half"] = QColor(255,255,255,150);   //half n half
	drinkColors["lime juice"] = QColor(255,255,255, 150);    //lime juice
	drinkColors["ginger beer"] = QColor(255,255,255, 150);   //ginger beer
	drinkColors["olive juice"] = QColor(255,255,255, 150);   //olive juice
	drinkColors["bitters"] = QColor(255,255,255, 150);       //bitters

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
	bdef.position.Set(iceDropSource.x(), DRINKVIEW_HEIGHT - iceDropSource.y());

	b2Body *iceBody = world->CreateBody(&bdef);
	iceBody->CreateFixture(&circle, 15);

	iceBodies.push_back(iceBody);
}

void LiquidModel::setVolume(int v)
{
	isDrinkEmpty = false;
	this->addLiquid(v); //determines the color of the liquor
}

void LiquidModel::setDrinkColor(QString drinkName)
{
	currentDrink = drinkName;
	qDebug() << currentDrink;
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

void LiquidModel::updateSimulation()
{
	if (isSimulationPaused)
		return;

	// Step the Box2D simulation
	world->Step(1.0f / 60.0f, 1, 3);

	//Set up Pixmap
	liquidPixmap.fill(Qt::transparent);
	QPainter painter(&liquidPixmap);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);

// THIS CODE IS FOR VISUALIZING THE PHYSICS BOUNDARIES BY DRAWING THEM.
#if 1
	// Get the body list from the world
	const b2Body* body = world->GetBodyList();

	// Iterate through all bodies in the world
	while (body)
	{
		b2Vec2 bodyPos = body->GetPosition();

		int x = bodyPos.x;
		int y = DRINKVIEW_HEIGHT - bodyPos.y;

		painter.drawPixmap(x, y, *iceTexture);
		// Check if the body is the container
		/*if (body->GetType() == b2_staticBody)
		{
			// Extract the fixtures from the body
			const b2Fixture* fixture = body->GetFixtureList();
			while (fixture)
			{
				// Check if the fixture is a polygon shape
				if (fixture->GetType() == b2Shape::e_polygon)
				{
					// Cast the shape to a polygon shape
					const b2PolygonShape* shape = dynamic_cast<const b2PolygonShape*>(fixture->GetShape());
					if (shape)
					{
						// Draw the polygon shape
						QPainterPath path;
						for (int i = 0; i < shape->GetVertexCount(); ++i)
						{
							b2Vec2 vertex = body->GetWorldPoint(shape->GetVertex(i));
							if (i == 0)
								path.moveTo(vertex.x, DRINKVIEW_HEIGHT - vertex.y);
							else
								path.lineTo(vertex.x, DRINKVIEW_HEIGHT - vertex.y);
						}
						path.closeSubpath();
						painter.setPen(Qt::transparent);
						painter.setBrush(Qt::transparent); // Set the color of the container
						painter.drawPath(path);
					}
				}
				// Move to the next fixture
				fixture = fixture->GetNext();
			}
		}*/
		// Move to the next body
		body = body->GetNext();
	}
#endif

	// Iterate through all particles in the current particle system
	for (int i = 0; i < liquidParticles->GetParticleCount(); ++i)
	{
		// Get particle position
		b2Vec2 particlePosition = liquidParticles->GetPositionBuffer()[i];
		//this needs to be converted back from a b2Color
		painter.setPen(liquidColor);
		painter.setBrush(liquidColor);
		painter.drawEllipse(QPointF(particlePosition.x, DRINKVIEW_HEIGHT - particlePosition.y), 4, 4); // Adjust the size as needed
	}

	painter.end();

	emit simulationUpdated(liquidPixmap);
}

void LiquidModel::addLiquid(int volume)
{
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


QColor LiquidModel::blendColorAlpha(QColor fgc, QColor bgc)
{
	// Taken from: https://stackoverflow.com/a/727339
	// Extremely suboptimal implementation.
	struct DClr { double r = 0.0; double g = 0.0; double b = 0.0; double a = 0.0; };
	DClr result;
	DClr bg{(double)bgc.red() / 255.0, (double)bgc.green() / 255.0, (double)bgc.blue() / 255.0, (double)bgc.alpha() / 255.0};
	DClr fg{(double)fgc.red() / 255.0, (double)fgc.green() / 255.0, (double)fgc.blue() / 255.0, (double)fgc.alpha() / 255.0};

	result.a = 1 - (1 - fg.a) * (1 - bg.a);

	if (result.a >= 1.0e-6) // If not fully transparent
	{
		result.r = fg.r * fg.a / result.a + bg.r * bg.a * (1 - fg.a) / result.a;
		result.g = fg.g * fg.a / result.a + bg.g * bg.a * (1 - fg.a) / result.a;
		result.b = fg.b * fg.a / result.a + bg.b * bg.a * (1 - fg.a) / result.a;
	}

	return QColor{uchar(255.0 * result.r), uchar(255.0 * result.g), uchar(255.0 * result.b), uchar(150)};// uchar(255.0 * result.a)
}
