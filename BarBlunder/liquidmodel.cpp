#include "liquidmodel.h"
#include "drinkview_size.h"

#include "glassware.h"

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <QDebug>

#include <Box2D/Box2D.h> // Please try not to include in any header. It increases the compile time.

LiquidModel::LiquidModel(QWidget *parent)
	: QObject{parent}
	, isDrinkEmpty{false}
	, volume{0}
	, isSimulationPaused{false}

    , liquidPixmap{DRINKVIEW_WIDTH, DRINKVIEW_HEIGHT}
    , blankPixmap{DRINKVIEW_WIDTH, DRINKVIEW_HEIGHT}
	, pouringSource{0.0f, 0.0f}

	, collisionBottom{nullptr}
	, collisionLeft{nullptr}
	, collisionRight{nullptr}
{
	b2Vec2 gravity(0.0f, 38.81f);
	world = new b2World(gravity);
	// setup timer
	updateTimer = new QTimer(this);
	connect(updateTimer, &QTimer::timeout, this, &LiquidModel::updateSimulation);
	updateTimer->start(16); // Update every 16 milliseconds (approximately 60 FPS)

	// setup colors for each drink
	drinkColors["whiskey"] = QColor(165, 113, 10, 190);		// Whiskey color
    drinkColors["tequila"] = QColor(172, 73, 25, 190);		// Tequila color
    drinkColors["rum"] = QColor(236, 233, 226, 20);			// Rum color
    drinkColors["vodka"] = QColor(236, 233, 226, 20);		// Vodka color
    drinkColors["gin"] = QColor(236, 233, 226, 20);			// Gin color
    drinkColors["kahlua"] = QColor(28, 1, 2, 220);			//kahlua

    //transparent pixmap for swapping
    //change to Qt::Transparent later
    blankPixmap.fill(Qt::black);

}

LiquidModel::~LiquidModel()
{
	delete world;
}

void LiquidModel::updateCollisionLayout(const Glassware &glassware)
{
	this->removeCollisionLayout();

	pouringSource.setX(glassware.getPhysicsPouringSource().x());
	pouringSource.setY(DRINKVIEW_HEIGHT - glassware.getPhysicsPouringSource().y());

	const QRectF &bottom_centre = glassware.getPhysicsBottomRect();
	const QRectF &left = glassware.getPhysicsLeftRect();
	const QRectF &right = glassware.getPhysicsRightRect();

	// Define the ground body (static)
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set((bottom_centre.left() + (bottom_centre.width() / 2)), (DRINKVIEW_HEIGHT - (bottom_centre.top() + (bottom_centre.height() / 2))));
	collisionBottom = world->CreateBody(&groundBodyDef);

	b2PolygonShape groundShape;
	groundShape.SetAsBox((bottom_centre.width() / 2), (bottom_centre.height() / 2)); // Ground dimensions
	collisionBottom->CreateFixture(&groundShape, 0.0f); // 0 density for static bodies

	// Define the left wall (static)
	b2BodyDef leftWallBodyDef;
	leftWallBodyDef.position.Set((left.left() + (left.width() / 2)), (DRINKVIEW_HEIGHT - (left.top() + (left.height() / 2)))); // Position of the left wall
	collisionLeft = world->CreateBody(&leftWallBodyDef);

	b2PolygonShape leftWallShape;
	leftWallShape.SetAsBox((left.width() / 2), (left.height() / 2)); // Dimensions of the left wall
	collisionLeft->CreateFixture(&leftWallShape, 0.0f); // 0 density for static bodies

	// Define the right wall (static)
	b2BodyDef rightWallBodyDef;
	rightWallBodyDef.position.Set((right.left() + (right.width() / 2)), (DRINKVIEW_HEIGHT - (right.top() + (right.height() / 2)))); // Position of the right wall
	collisionRight = world->CreateBody(&rightWallBodyDef);

	b2PolygonShape rightWallShape;
	rightWallShape.SetAsBox((right.width() / 2), (right.height() / 2)); // Dimensions of the right wall
	collisionRight->CreateFixture(&rightWallShape, 0.0f); // 0 density for static bodies
}

void LiquidModel::removeCollisionLayout()
{
	if (collisionBottom)
		world->DestroyBody(collisionBottom);

	if (collisionLeft)
		world->DestroyBody(collisionLeft);

	if (collisionRight)
		world->DestroyBody(collisionRight);
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
	// Iterate through each stored particle system and clear it
	for (auto particleSystem : particleSystemsList)
	{
		if (particleSystem)
		{
			int particleCount = particleSystem->GetParticleCount();

			for (int i = 0; i < particleCount; ++i)
			{
				particleSystem->DestroyParticle(i, true);
			}
		}
	}

	// Clear the vector storing all particle systems
	particleSystemsList.clear();

	isDrinkEmpty = true;
	emit emptyLiquid();
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

	// THIS CODE IS FOR VISUALIZING THE PHYSICS BOUNDARIES BY DRAWING THEM.

	liquidPixmap.fill(Qt::transparent);


	QPainter painter(&liquidPixmap);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setCompositionMode(QPainter::CompositionMode_Source);

#if 1
	// Get the body list from the world
	const b2Body* body = world->GetBodyList();

	// Iterate through all bodies in the world
	while (body)
	{
		// Check if the body is the container
		if (body->GetType() == b2_staticBody)
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
		}
		// Move to the next body
		body = body->GetNext();
	}
#endif

	// Get the particle system list from the world
	const b2ParticleSystem *particleSystemList = world->GetParticleSystemList();

    // // Retrieve the color corresponding to the current drink
    // QColor ingredientColor = drinkColors[currentDrink];

	// Iterate through all particle systems
	for (const b2ParticleSystem* particleSystem = particleSystemList; particleSystem; particleSystem = particleSystem->GetNext())
	{
		// Iterate through all particles in the current particle system
		for (int i = 0; i < particleSystem->GetParticleCount(); ++i)
		{

			// Get particle position
			b2Vec2 particlePosition = particleSystem->GetPositionBuffer()[i];
            //this needs to be converted back from a b2Color
            QColor liquorColor(particleSystem->GetColorBuffer()[i].GetColor().r * 255,
                               particleSystem->GetColorBuffer()[i].GetColor().g * 255,
                               particleSystem->GetColorBuffer()[i].GetColor().b * 255);
            painter.setPen(liquorColor);
            painter.setBrush(liquorColor);
			painter.drawEllipse(QPointF(particlePosition.x, DRINKVIEW_HEIGHT - particlePosition.y), 4, 4); // Adjust the size as needed
		}
	}

	painter.end();

	emit simulationUpdated(liquidPixmap);
}

void LiquidModel::setupLiquidParticleSystem()
{
	b2ParticleSystemDef particleSystemDef;

	// Define the liquid particle system parameters (idk how many are neccessary here just was messing around)
	particleSystemDef.gravityScale = -9.0f;
    particleSystemDef.radius = 1.75f; // Particle radius
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
	particleSystemDef.density = 1.0;

	// Create the liquid particle system
	particleSystem = world->CreateParticleSystem(&particleSystemDef);
	particleSystem->SetGravityScale(-9.0f);
}

void LiquidModel::addLiquid(int volume)
{
	this->setupLiquidParticleSystem();
	particleSystemsList.push_back(particleSystem);

	int numParticlesX = 3; // number of particles spawned horizontally
	int numParticlesY = 3; // number of particles spawned vertically
	float particleSpacingX = 4.5f; // Adjust the spacing between particles in the x direction
	float particleSpacingY = 4.5f; // Adjust the spacing between particles in the y direction

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
                    QColor drinkcolor = drinkColors.value(currentDrink);
                    // convert to a b2color to be used with particleDef
                    b2ParticleColor b2c;
                    b2c.Set(drinkcolor.red(),drinkcolor.green(),drinkcolor.blue(),drinkcolor.alpha());

                    particleDef.color = b2c;

					// Spawn the particle
					particleSystem->CreateParticle(particleDef);
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

void LiquidModel::hideLiquid(){

    emit removeLiquid();
}

void LiquidModel::exposeLiquid(){
   emit simulationUpdated(liquidPixmap);

}
