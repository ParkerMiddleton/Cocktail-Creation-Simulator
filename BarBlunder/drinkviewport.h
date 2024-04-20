#ifndef DRINKVIEWPORT_H
#define DRINKVIEWPORT_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>

// Forward Declaration.
class LiquidModel;

class DrinkViewport : public QGraphicsView
{
	Q_OBJECT

public:
	DrinkViewport(QWidget *parent = nullptr);
	~DrinkViewport();

	void initializeConnections(LiquidModel *liquid);

public slots:
	void updateLiquidDisplay(const QPixmap &pixmap);
	void updateGlasswareImage(const QPixmap &pixmap);

private:
	static constexpr int WIDTH = 256;
	static constexpr int HEIGHT = 280;

	QGraphicsScene gScene;
	QGraphicsItemGroup gItemGroup;

	QGraphicsPixmapItem gLiquid;
	QGraphicsPixmapItem gGlass;

};

#endif // DRINKVIEWPORT_H
