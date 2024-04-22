#ifndef LIQUID_H
#define LIQUID_H

#include <Box2D/Common/b2Math.h>
#include <Box2D/Particle/b2Particle.h>

#include <QObject>

#include <QMap>
#include <QPixmap>

// Forward declaration.
class Glassware;

class b2World;
class b2Body;
class b2ParticleSystem;

class LiquidModel : public QObject
{
	Q_OBJECT

public:
	explicit LiquidModel(QWidget *parent = nullptr);
	~LiquidModel();

	void updateGlassware(const Glassware &glassware);
	void removeGlassware();
	void empty();

	void addIce();
	void updateDrinkColor(const QString &drinkName);
	void dashPour(int volume);
	void stir();

	void startPouring();
	void stopPouring();

	void startShaking();
	void stopShaking();

	void update(int deltaTime);

signals:
	void liquidEmptied();
	void simulationUpdated(const QPixmap &pixmap);

private:
	static constexpr float GRAVITY_SCALE = 30.0f;
	static constexpr float PARTICLE_RADIUS = 3.75f;
	static constexpr int PARTICLES_NUM_SPAWN_VERTICAL = 4;

	bool isStirring;
	bool isPouring;
	int pouringElapsedTime;

	QMap<QString, b2ParticleColor> drinkColors;
	QString currentDrink;

	QPixmap liquidPixmap;
	QPixmap *iceTexture;

	// Box2D
	b2World *world;
	b2Body *collisionBody;
	b2Vec2 pouringSource;

	b2ParticleSystem *liquidParticles;
	QList<b2Body*> iceBodies;

	void draw();

};

#endif // LIQUID_H
