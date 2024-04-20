#include "liquidmodel.h"

#include <QPainterPath>

LiquidModel::LiquidModel(QWidget *parent)
	: QObject{parent}
	, isDrinkEmpty{false}
	, volume{0}
	, liquidPixmap{WIDTH, HEIGHT}

	, physicsGravity{0.0f, 38.81f}
	, physicsWorld{physicsGravity}
{
	liquidPixmap.fill(Qt::transparent);

	this->setupBox2D();

	// setup timer
	updateTimer = new QTimer(this);
	connect(updateTimer, &QTimer::timeout, this, &LiquidModel::updateSimulation);
	updateTimer->start(16); // Update every 16 milliseconds (approximately 60 FPS)

	// setup colors for each drink
	drinkColors["whiskey"] = QColor(165, 113, 10, 190);		// Whiskey color
	drinkColors["tequila"] = QColor(240, 230, 14, 190);		// Tequila color
	drinkColors["rum"] = QColor(139, 69, 19, 190);			// Rum color
	drinkColors["vodka"] = QColor(240, 255, 13, 190);		// Vodka color
	drinkColors["gin"] = QColor(173, 255, 30, 190);			// Gin color
	drinkColors["kahlua"] = QColor(70, 40, 0, 190);			//kahlua
}

LiquidModel::~LiquidModel()
{
}

void LiquidModel::clearDrink()
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
}

void LiquidModel::setupLiquidParticleSystem()
{
	// Define the liquid particle system parameters (idk how many are neccessary here just was messing around)
	particleSystemDef.gravityScale = -9.0f;
	particleSystemDef.radius = 4.25f; // Particle radius
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
	particleSystem = physicsWorld.CreateParticleSystem(&particleSystemDef);
	particleSystem->SetGravityScale(-9.0f);
}

void LiquidModel::addLiquid(int volume)
{
	// spawn ball of liquid just above bottom of the glass.
	//b2Vec2 cupCenter(120.0f, 77.0f);
	b2Vec2 cupCenter(120, HEIGHT - 165);
	//b2Vec2 cupCenter(122 / SCALE, (HEIGHT - 130) / SCALE);

	setupLiquidParticleSystem();
	particleSystemsList.push_back(particleSystem);

	int numParticlesX = 3; // number of particles spawned horizontally
	int numParticlesY = 3; // number of particles spawned vertically
	float particleSpacingX = 4.5f; // Adjust the spacing between particles in the x direction
	float particleSpacingY = 4.5f; // Adjust the spacing between particles in the y direction

	QTimer* timer = new QTimer(); // Allocate QTimer dynamically
	connect(timer, &QTimer::timeout, this, [=]() {
		// Spawn particles for 3 seconds
		static float elapsedTime = 0.0f;
		// attempting to use volume
		if (elapsedTime < volume && !isDrinkEmpty) {
			// Spawn particles in the loop
			for (int i = 0; i < numParticlesX; ++i) {
				for (int j = 0; j < numParticlesY; ++j) {
					b2Vec2 particlePosition;
					particlePosition.x = cupCenter.x - (numParticlesX / 2.0f) * particleSpacingX + i * particleSpacingX;
					particlePosition.y = cupCenter.y - (numParticlesY / 2.0f) * particleSpacingY + j * particleSpacingY;

					// Create a particle
					b2ParticleDef particleDef;
					particleDef.flags = b2_elasticParticle;
					particleDef.position = particlePosition; // Particle position its spawned at
					particleDef.color.Set(255, 0, 0, 255); // Particle color (doesnt seem to be doing anything)

					// Spawn the particle
					particleSystem->CreateParticle(particleDef);
				}
			}
			elapsedTime += 0.1; // Increase elapsed time
		} else {
			timer->stop(); // Stop the timer after 3 seconds
			delete timer; // delete timer
			elapsedTime = 0.0;
		}
	});

	timer->start(100);
}

void LiquidModel::updateSimulation()
{
	// Step the Box2D simulation
	physicsWorld.Step(1.0f / 60.0f, 1, 3);

	// THIS CODE IS FOR VISUALIZING THE PHYSICS BOUNDARIES BY DRAWING THEM.

	liquidPixmap.fill(Qt::transparent);

	QPen pen;
	pen.setWidth(1);
	QPainter painter(&liquidPixmap);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.setRenderHint(QPainter::Antialiasing);

	// Get the particle system list from the world
	const b2ParticleSystem* particleSystemList = physicsWorld.GetParticleSystemList();

	// Retrieve the color corresponding to the current drink
	QColor ingredientColor = drinkColors[currentDrink];

	// Iterate through all particle systems
	for (const b2ParticleSystem* particleSystem = particleSystemList; particleSystem; particleSystem = particleSystem->GetNext())
	{
		// Iterate through all particles in the current particle system
		for (int i = 0; i < particleSystem->GetParticleCount(); ++i)
		{
			// Get particle position
			b2Vec2 particlePosition = particleSystem->GetPositionBuffer()[i];
			pen.setColor(ingredientColor);
			painter.setPen(pen);
			painter.setBrush(ingredientColor);
			painter.drawEllipse(QPointF(particlePosition.x, HEIGHT - particlePosition.y), 4, 4); // Adjust the size as needed
		}
	}

	painter.end();

	emit simulationUpdated(liquidPixmap);
}

void LiquidModel::setVolume(int v)
{
	isDrinkEmpty = false;
	this->addLiquid(v);
}

void LiquidModel::setDrinkColor(QString drinkName)
{
	currentDrink = drinkName;
}

void LiquidModel::emptyDrink()
{
	this->clearDrink();
	isDrinkEmpty = true;
	emit emptyLiquid();
}

void LiquidModel::setupBox2D()
{
	QRectF bottom_centre(89,  236, 64, 15);
	QRectF left(80, 168, 9, 68);
	QRectF right(153, 168, 9, 68);

	/*QRectF bottom_centre(89, 236, 64, 15);
	QRectF left(80, 168, 9, 68);
	QRectF right(153, 168, 9, 68);*/

	// Define the ground body (static)
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set((bottom_centre.left() + (bottom_centre.width() / 2)), (HEIGHT - (bottom_centre.top() + (bottom_centre.height() / 2))));
	b2Body* groundBody = physicsWorld.CreateBody(&groundBodyDef);

	b2PolygonShape groundShape;
	groundShape.SetAsBox((bottom_centre.width() / 2), (bottom_centre.height() / 2)); // Ground dimensions
	groundBody->CreateFixture(&groundShape, 0.0f); // 0 density for static bodies

	// Define the left wall (static)
	b2BodyDef leftWallBodyDef;
	leftWallBodyDef.position.Set((left.left() + (left.width() / 2)), (HEIGHT - (left.top() + (left.height() / 2)))); // Position of the left wall
	b2Body* leftWallBody = physicsWorld.CreateBody(&leftWallBodyDef);

	b2PolygonShape leftWallShape;
	leftWallShape.SetAsBox((left.width() / 2), (left.height() / 2)); // Dimensions of the left wall
	leftWallBody->CreateFixture(&leftWallShape, 0.0f); // 0 density for static bodies

	// Define the right wall (static)
	b2BodyDef rightWallBodyDef;
	rightWallBodyDef.position.Set((right.left() + (right.width() / 2)), (HEIGHT - (right.top() + (right.height() / 2)))); // Position of the right wall
	b2Body* rightWallBody = physicsWorld.CreateBody(&rightWallBodyDef);

	b2PolygonShape rightWallShape;
	rightWallShape.SetAsBox((right.width() / 2), (right.height() / 2)); // Dimensions of the right wall
	rightWallBody->CreateFixture(&rightWallShape, 0.0f); // 0 density for static bodies
}
