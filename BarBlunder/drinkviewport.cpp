#include "drinkviewport.h"

#include "liquidmodel.h"

DrinkViewport::DrinkViewport(QWidget *parent)
	: QGraphicsView{parent}
	, gScene(0, 0, WIDTH, HEIGHT)
{
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setTransformationAnchor(QGraphicsView::NoAnchor);

	gItemGroup.addToGroup(&gLiquid);
	gItemGroup.addToGroup(&gGlass);
	gScene.addItem(&gItemGroup);
	this->setScene(&gScene);
}

DrinkViewport::~DrinkViewport()
{

}

void DrinkViewport::initializeConnections(LiquidModel *liquid)
{
	connect(liquid, &LiquidModel::simulationUpdated,
			this, &DrinkViewport::updateLiquidDisplay);
}

void DrinkViewport::updateLiquidDisplay(const QPixmap &pixmap)
{
	gLiquid.setPixmap(pixmap);
}

void DrinkViewport::updateGlasswareImage(const QPixmap &pixmap)
{
	gGlass.setPixmap(pixmap);
}
