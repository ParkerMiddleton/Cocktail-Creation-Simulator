#ifndef DRINKVIEW_H
#define DRINKVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>

// Forward Declaration.
class BarModel;
class Glassware;

class DrinkView : public QGraphicsView
{
	Q_OBJECT

public:
	DrinkView(QWidget *parent = nullptr);
	~DrinkView();

	void initializeConnections(BarModel *bar);

public slots:
	void updateLiquidDisplay(const QPixmap &pixmap);
    void removeLiquidDisplay();
	void updateGlasswareImage(const Glassware &glassware);
	void removeGlasswareImage();

private:
	QGraphicsScene gScene;
	QGraphicsItemGroup gItemGroup;

	QGraphicsPixmapItem gMug;
	QGraphicsPixmapItem gLiquid;
	QGraphicsPixmapItem gGlass;

	QPixmap emptyPixmap;

};

#endif // DRINKVIEW_H
