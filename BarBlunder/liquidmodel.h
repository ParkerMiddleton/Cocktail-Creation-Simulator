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

/// @brief Responsible for the main drink physics and liquid. Part of Model.
class LiquidModel : public QObject
{
	Q_OBJECT

public:
	/// @brief Constructor.
	explicit LiquidModel(QWidget *parent = nullptr);

	/// @brief Destructor.
	~LiquidModel();

	/// @brief Removes the current collision and liquid,
	/// and intializes collision of the given glassware.
	/// @param glassware Glassware to initialize the collision of.
	void updateGlassware(const Glassware &glassware);

	/// @brief Removes the current collision and liquid,
	void removeGlassware();

	/// @brief Removes liquid and ice cubes.
	void empty();

	/// @brief Schedules new ice cubes to place in the drink.
	void addIce();

	/// @brief Sets the color of the new liquid to pour.
	/// @param drinkName Drink of the color of which to take.
	void updateDrinkColor(const QString &drinkName);

	/// @brief Schedules the given amount of liquid to pour.
	/// If already pouring, adds the given amount to the total amount to pour.
	/// @param ounce Amount to schedule.
	void pour(int ounce);

	/// @brief Mixes the color of different liquids in the drink.
	void mix();

	/// @brief Updates the physics world, checks the scheduled tasks, and updates what should be drawn.
	/// @param deltaTime Time between the last and current update.
	void update(int deltaTime);

signals:
	/// @brief Notifies the removal of liquid and ice cubes.
	void liquidEmptied();

	/// @brief Notifies about the new liquid and ice cubes draw data.
	/// @param pixmap QPixmap with draw data.
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

	/// @brief Updates the liquid and ice cubes draw data.
	void draw();

	/// @brief Spawns new particles of liquid.
	void spawnParticles();

	/// @brief If scheduled, adds new ice cube.
	void checkScheduledAddIceBody();

	/// @brief If scheduled, removes all ice cubes.
	void checkSheduledRemoveIceBodies();

};

#endif // LIQUID_H
