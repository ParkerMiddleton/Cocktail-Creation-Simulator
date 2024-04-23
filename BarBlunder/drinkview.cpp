#include "drinkview.h"
#include "drinkview_size.h"

#include "barmodel.h"
#include "glassware.h"

DrinkView::DrinkView(QWidget *parent)
	: QGraphicsView{parent}
	, gScene(0, 0, DRINKVIEW_WIDTH, DRINKVIEW_HEIGHT)
	, emptyPixmap{256, 280}
	, currentGlassware{nullptr}
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

	this->rotate(5.0f);
	this->rotate(-5.0f);
	this->resetTransform();
}

DrinkView::~DrinkView()
{

}

void DrinkView::initializeConnections(BarModel *bar)
{
	connect(bar->liquidModel(), &LiquidModel::simulationUpdated,
			this, &DrinkView::updateLiquidDisplay);

	connect(bar, &BarModel::newGlasswarePlaced,
			this, &DrinkView::setNewGlassware);

	connect(bar, &BarModel::currentGlasswareRemoved,
			this, &DrinkView::removeGlasswareDisplay);

	connect(bar, &BarModel::currentGlasswareUpdated,
			this, &DrinkView::updateCurrentGlasswareDisplay);
}

void DrinkView::updateLiquidDisplay(const QPixmap &liquid)
{
	gLiquid.setPixmap(liquid);
}

void DrinkView::setNewGlassware(Glassware *glassware)
{
	currentGlassware = glassware;

	this->updateCurrentGlasswareDisplay();
}

void DrinkView::updateCurrentGlasswareDisplay()
{
	if (currentGlassware)
	{
		if (currentGlassware->isTransparent())
		{
			gMug.setPixmap(emptyPixmap);
			gGlass.setPixmap(currentGlassware->getDisplayPixmap());
		}
		else
		{
			gMug.setPixmap(currentGlassware->getDisplayPixmap());
			gGlass.setPixmap(emptyPixmap);
		}
	}
}

void DrinkView::removeGlasswareDisplay()
{
	gMug.setPixmap(emptyPixmap);
	gGlass.setPixmap(emptyPixmap);
}
