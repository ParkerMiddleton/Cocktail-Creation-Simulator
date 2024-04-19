#include "liquidmodel.h"
#include <QTimer>

LiquidModel::LiquidModel(QWidget *parent) : QObject(parent) {

}

LiquidModel::~LiquidModel()
{
    if (m_world) {
        delete m_world;
        m_world = nullptr;
    }
}

void LiquidModel::clearDrink() {
    // Iterate through each stored particle system and clear it
    for (auto particleSystem : particleSystemsList) {
        if (particleSystem) {
            int particleCount = particleSystem->GetParticleCount();
            for (int i = 0; i < particleCount; ++i) {
                particleSystem->DestroyParticle(i, true);
            }
        }
    }
    // Clear the vector storing all particle systems
    particleSystemsList.clear();
}

void LiquidModel::setupLiquidParticleSystem() {
    // Define the liquid particle system parameters (idk how many are neccessary here just was messing around)
    particleSystemDef.gravityScale = -9.0f;
    particleSystemDef.radius = 2.25f; // Particle radius
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
    particleSystem = m_world->CreateParticleSystem(&particleSystemDef);
    particleSystem->SetGravityScale(-9.0f);
}

void LiquidModel::addLiquid(int volume) {

    // spawn ball of liquid just above bottom of the glass.
    b2Vec2 cupCenter(607.0f, 370.0f);

    setupLiquidParticleSystem();
    particleSystemsList.push_back(particleSystem);

    int numParticlesX = 5; // number of particles spawned horizontally
    int numParticlesY = 5; // number of particles spawned vertically
    float particleSpacingX = 3.0f; // Adjust the spacing between particles in the x direction
    float particleSpacingY = 3.0f; // Adjust the spacing between particles in the y direction

    QTimer* timer = new QTimer(); // Allocate QTimer dynamically
    connect(timer, &QTimer::timeout, this, [=]() {
        // Spawn particles for 3 seconds
        static float elapsedTime = 0.0f;
        // attempting to use volume
        if (elapsedTime < volume) {
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


void LiquidModel::setupBox2D()
{
    // Create the Box2D world
    b2Vec2 gravity(0.0f, -38.81f);
    m_world = new b2World(gravity);

    // Define the ground body (static)
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(602.0f, 380.0f);
    b2Body* groundBody = m_world->CreateBody(&groundBodyDef);

    b2PolygonShape groundShape;
    groundShape.SetAsBox(32.0f, 0.5f); // Ground dimensions
    groundBody->CreateFixture(&groundShape, 0.0f); // 0 density for static bodies

    // Define the left wall (static)
    b2BodyDef leftWallBodyDef;
    leftWallBodyDef.position.Set(570.0f, 350.0f); // Position of the left wall
    b2Body* leftWallBody = m_world->CreateBody(&leftWallBodyDef);

    b2PolygonShape leftWallShape;
    leftWallShape.SetAsBox(0.5f, 70.0f); // Dimensions of the left wall
    leftWallBody->CreateFixture(&leftWallShape, 0.0f); // 0 density for static bodies

    // Define the right wall (static)
    b2BodyDef rightWallBodyDef;
    rightWallBodyDef.position.Set(633.0f, 350.0f); // Position of the right wall
    b2Body* rightWallBody = m_world->CreateBody(&rightWallBodyDef);

    b2PolygonShape rightWallShape;
    rightWallShape.SetAsBox(0.5f, 70.0f); // Dimensions of the right wall
    rightWallBody->CreateFixture(&rightWallShape, 0.0f); // 0 density for static bodies
}

