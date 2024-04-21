#ifndef DRINKVIEW_H
#define DRINKVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>

// Forward Declaration.
/// @class the Bar model class
class BarModel;
/// @class the glassware clas
class Glassware;

/**
 * @brief The class for the drink view
 */
class DrinkView : public QGraphicsView
{
	Q_OBJECT

public:
    /**
     * @brief DrinkView - the drink view
     * @param parent - the parent class for user inteface you are using; default is null
     */
	DrinkView(QWidget *parent = nullptr);
    /// @brief the destructor for the drink view
	~DrinkView();
    /**
     * @brief initializeConnections - inits the connections in the game software
     * @param bar - a pointer to the bar model instance being used
     */
	void initializeConnections(BarModel *bar);

public slots:
    /**
     * @brief updateLiquidDisplay -this slot updated the display of the liquid
     * @param pixmap -  a QPixmap of the pixels in the game interface
     */
	void updateLiquidDisplay(const QPixmap &pixmap);
    /// @brief this slot removes the liquid
    void removeLiquidDisplay();
    /**
     * @brief updateGlasswareImage - a updsted image of the glass ware passed in updates of the drink view
     * @param glassware -  a const ref to the glassware type used
     */
	void updateGlasswareImage(const Glassware &glassware);
    /// @brief this slot removes the glassware image
	void removeGlasswareImage();

private:
    /// @brief the graphics scene
	QGraphicsScene gScene;
    /// @brief the graphics item group
	QGraphicsItemGroup gItemGroup;
    /// @brief the graphics pixel map of the mug
	QGraphicsPixmapItem gMug;
    /// @brief the graphics pixel map of the liquid
	QGraphicsPixmapItem gLiquid;
    /// @brief the graphics pixel map of the glass
	QGraphicsPixmapItem gGlass;
    /// @brief an empty pixmap
	QPixmap emptyPixmap;

};

#endif // DRINKVIEW_H
