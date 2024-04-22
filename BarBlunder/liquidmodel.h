#ifndef LIQUID_H
#define LIQUID_H

#include <QObject>

#include <QMap>
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

	void addIce();
	void addLiquid(int volume);

	void clear();

	void updateDrinkColor(const QString &drinkName);
	void setIsSimulationPaused(bool state);

signals:
	void liquidEmptied();
	void simulationUpdated(const QPixmap &pixmap);

private slots:
	void updateSimulation();

private:
	QTimer *simulationUpdateTimer;
	bool isSimulationPaused;
	bool isDrinkEmpty;

	QMap<QString, QColor> drinkColors;
	QString currentDrink;
	QColor liquidColor;

	QPointF pouringSource;

	QPixmap liquidPixmap;
	QPixmap *iceTexture;

	// Box2D
	b2World *world;
	b2Body *collisionBody;
	b2ParticleSystem *liquidParticles;
	QList<b2Body*> iceBodies;

	QColor blendColorAlpha(QColor fgc, QColor bgc);

};

#endif // LIQUID_H
