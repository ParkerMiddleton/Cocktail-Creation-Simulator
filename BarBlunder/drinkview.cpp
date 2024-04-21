#include "drinkview.h"
#include "drinkview_size.h"

#include "barmodel.h"
#include "glassware.h"

DrinkView::DrinkView(QWidget *parent)
	: QGraphicsView{parent}
	, gScene(0, 0, DRINKVIEW_WIDTH, DRINKVIEW_HEIGHT)
	, emptyPixmap{256, 280}
{
	emptyPixmap.fill(Qt::transparent);

	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setTransformationAnchor(QGraphicsView::NoAnchor);

	gItemGroup.addToGroup(&gMug);
	gItemGroup.addToGroup(&gLiquid);
	gItemGroup.addToGroup(&gGlass);
	gScene.addItem(&gItemGroup);
	this->setScene(&gScene);
}

DrinkView::~DrinkView()
{

}

void DrinkView::initializeConnections(BarModel *bar)
{
	connect(bar->liquidModel(), &LiquidModel::simulationUpdated,
			this, &DrinkView::updateLiquidDisplay);

    connect(bar->liquidModel(), &LiquidModel::removeLiquid,
            this, &DrinkView::removeLiquidDisplay);

	connect(bar, &BarModel::glasswareUpdated,
			this, &DrinkView::updateGlasswareImage);

	connect(bar, &BarModel::glasswareRemoved,
			this, &DrinkView::removeGlasswareImage);
}

void DrinkView::updateLiquidDisplay(const QPixmap &pixmap)
{
	gLiquid.setPixmap(pixmap);
}

void DrinkView::removeLiquidDisplay(){
    gLiquid.setPixmap(emptyPixmap);
}

void DrinkView::updateGlasswareImage(const Glassware &glassware)
{
	if (glassware.isTransparent())
	{
		gMug.setPixmap(emptyPixmap);
		gGlass.setPixmap(glassware.getDisplayPixmap());
	}
	else
	{
		gMug.setPixmap(glassware.getDisplayPixmap());
		gGlass.setPixmap(emptyPixmap);
	}
}

void DrinkView::removeGlasswareImage()
{
	gMug.setPixmap(emptyPixmap);
	gGlass.setPixmap(emptyPixmap);
}
