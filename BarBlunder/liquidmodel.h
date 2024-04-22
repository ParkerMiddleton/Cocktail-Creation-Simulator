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

	void setVolume(int v);
	void setDrinkColor(QString drinkName);
	void clear();

	void setIsSimulationPaused(bool state);

signals:
	void liquidEmptied();
	void simulationUpdated(const QPixmap &pixmap);

private slots:
	void updateSimulation();

private:
	bool isDrinkEmpty;
	int volume;
	QMap<QString, QColor> drinkColors;
	QColor liquidColor;
	QString currentDrink;

	QTimer *updateTimer;
	bool isSimulationPaused;

	QPixmap liquidPixmap;
    QPixmap blankPixmap;

	QPointF pouringSource;
	QPointF iceDropSource;

	QPixmap *iceTexture;

	// Box2D
	b2World *world;
	b2Body *collisionBody;
	b2ParticleSystem *liquidParticles;
	QList<b2Body*> iceBodies;

	void addLiquid(int volume);
	void setupLiquidParticleSystem();
	QColor blendColorAlpha(QColor fgc, QColor bgc);

};

#endif // LIQUID_H
