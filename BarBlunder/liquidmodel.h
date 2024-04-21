#ifndef LIQUID_H
#define LIQUID_H

#include <QObject>

#include <Qmap>
#include <QTimer>
#include <QPixmap>
#include <QPointF>

// Forward declaration.
class Glassware;

class b2World;
class b2Body;
class b2Vec2;
class b2ParticleSystem;

class LiquidModel : public QObject
{
	Q_OBJECT

public:
	explicit LiquidModel(QWidget *parent = nullptr);
	~LiquidModel();

	void updateCollisionLayout(const Glassware &glassware);
	void removeCollisionLayout();

	void setVolume(int v);
	void setDrinkColor(QString drinkName);
	void clear();

	void setIsSimulationPaused(bool state);
    void hideLiquid();
    void exposeLiquid();

signals:
	void emptyLiquid();
    void removeLiquid();
	void simulationUpdated(const QPixmap &pixmap);

private slots:
	void updateSimulation();

private:
	bool isDrinkEmpty;
	int volume;
	QMap<QString, QColor> drinkColors;
	QString currentDrink;

	QTimer *updateTimer;
	bool isSimulationPaused;

	QPixmap liquidPixmap;
    QPixmap blankPixmap;

	QPointF pouringSource;

	// Box2D
	b2World *world;
	b2Body *collisionBottom;
	b2Body *collisionLeft;
	b2Body *collisionRight;
	b2ParticleSystem *particleSystem;
    std::vector<b2ParticleSystem*> particleSystemsList;

	void setupLiquidParticleSystem();
	void addLiquid(int volume);

};

#endif // LIQUID_H
