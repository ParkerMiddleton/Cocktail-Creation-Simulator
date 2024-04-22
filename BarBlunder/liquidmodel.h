#ifndef LIQUID_H
#define LIQUID_H

#include <Box2D/Common/b2Math.h>
#include <Box2D/Particle/b2Particle.h>
#include <Box2D/Particle/b2ParticleSystem.h>

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
	void pour(int ounce);
	void mix();

	void update(int deltaTime);

signals:
	void liquidEmptied();
	void simulationUpdated(const QPixmap &pixmap);

private:
	static constexpr float GRAVITY_SCALE = 30.0f;
	static constexpr float PARTICLE_RADIUS = 3.75f;
	static constexpr int PARTICLES_NUM_SPAWN_VERTICAL = 4;
	static constexpr int DROPS_INTERVAL = 80;
	static constexpr int OUNCE_POURING_DURATION = 1000;

	QMap<QString, b2ParticleColor> drinkColors;
	QString currentDrink;

	bool isStirring;
	bool isPouring;
	int pouringElapsedTime;
	int timeToPour;

	QPixmap liquidPixmap;
	QPixmap *iceTexture;

	// Box2D
	bool addIceBody;
	bool removeIceBodies;

	b2Vec2 gravity;
	b2ParticleSystemDef particleSystemDef;
	b2Vec2 pouringSource;

	b2World *world;
	b2ParticleSystem *liquidParticles;
	QList<b2Body*> iceBodies;

	void draw();

	void spawnParticles();
	void checkScheduledAddIceBody();
	void checkSheduledRemoveIceBodies();

};

#endif // LIQUID_H
