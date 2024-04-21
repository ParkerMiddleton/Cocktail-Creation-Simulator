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

    , liquidPixmap{DRINKVIEW_WIDTH, DRINKVIEW_HEIGHT}
    , blankPixmap{DRINKVIEW_WIDTH, DRINKVIEW_HEIGHT}
	, pouringSource{0.0f, 0.0f}

	, collisionBody{nullptr}
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
    drinkColors["burbon"] = QColor(255,255,255,255);        //burbon
    drinkColors["tripe sec"] = QColor(255,255,255,255);     //triple sec
    drinkColors["sweet n sour"] = QColor(255,255,255,255);  //sweet n sour
    drinkColors["grenadine"] = QColor(255,255,255,255);     //grenadine
    drinkColors["simple syrup"] = QColor(255,255,255,255);  //simpe syrup
    drinkColors["agave nectar"] = QColor(255,255,255,255);  //agave nectar
    drinkColors["sprite"] = QColor(255,255,255,255);        //sprite
    drinkColors["coke"] = QColor(255,255,255,255);          //coke
    drinkColors["half n half"] = QColor(255,255,255,255);   //half n half
    drinkColors["lime juice"] = QColor(255,255,255,255);    //lime juice
    drinkColors["ginger beer"] = QColor(255,255,255,255);   //ginger beer
    drinkColors["olive juice"] = QColor(255,255,255,255);   //olive juice
    drinkColors["bitters"] = QColor(255,255,255,255);       //bitters

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

    // //trying this

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
                               particleSystem->GetColorBuffer()[i].GetColor().b * 255,
                               150);
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
