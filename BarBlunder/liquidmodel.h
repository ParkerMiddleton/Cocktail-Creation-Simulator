#ifndef LIQUID_H
#define LIQUID_H

#include <QObject>
#include <QWidget>

#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include <QPen>
#include <Qmap>

#include <QPixmap>

#include <Box2D/Box2D.h>

class LiquidModel : public QObject
{
	Q_OBJECT // Macro needed for signal-slot mechanism

public:
	explicit LiquidModel(QWidget *parent = nullptr); // Constructor with parent widget
	~LiquidModel();

	void setupLiquidParticleSystem();
	void clearDrink();
	void setupBox2D(); // Initiliaze glasses.

public slots:
	void addLiquid(int volume);
	void setVolume(int v);
	void setDrinkColor(QString drinkName);
	void emptyDrink();

signals:
	void emptyLiquid();
	void simulationUpdated(const QPixmap &pixmap);

private slots:
	void updateSimulation();

private:
	static constexpr int WIDTH = 256;
	static constexpr int HEIGHT = 280;

	bool isDrinkEmpty;
	int volume;
	QMap<QString, QColor> drinkColors;
	QString currentDrink;

	QTimer *updateTimer;
	QPixmap liquidPixmap;

	// Box2D
	b2ParticleSystemDef particleSystemDef;
	b2ParticleSystem* particleSystem;
	b2Vec2 physicsGravity;
	b2World physicsWorld;
	std::vector<b2ParticleSystem*> particleSystemsList;

};

#endif // LIQUID_H
