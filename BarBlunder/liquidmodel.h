#ifndef LIQUID_H
#define LIQUID_H

#include <Box2D/Particle/b2Particle.h>

#include <QObject>

#include <QMap>
#include <QTimer>
#include <QPixmap>
#include <QPointF>

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

	void updateCollisionLayout(const Glassware &glassware);
	void removeCollisionLayout();

	void addIce();
	void addLiquid(int volume);

	void clear();

	void updateDrinkColor(const QString &drinkName);
	void setIsSimulationPaused(bool state);

	void stir();

	void update();

signals:
	void liquidEmptied();
	void simulationUpdated(const QPixmap &pixmap);

private:
	static constexpr float GRAVITY_SCALE = 30.0f;
	static constexpr float PARTICLE_RADIUS = 3.75f;

	QTimer *simulationUpdateTimer;
	bool isSimulationPaused;
	bool isDrinkEmpty;

	QMap<QString, b2ParticleColor> drinkColors;
	QString currentDrink;

	QPointF pouringSource;

	QPixmap liquidPixmap;
	QPixmap *iceTexture;

	bool isMixing;

	// Box2D
	b2World *world;
	b2Body *collisionBody;
	b2ParticleSystem *liquidParticles;
	QList<b2Body*> iceBodies;

	void draw();

};

#endif // LIQUID_H
