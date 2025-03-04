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

/// @brief Displays liquid with the glassware. Part of View.
/// @extends QGraphicsView
class DrinkView : public QGraphicsView
{
	Q_OBJECT

public:
	/// @brief Constructor.
	DrinkView(QWidget *parent = nullptr);

	/// @brief Set ups connections between this widget and the given bar model object.
	/// Must be called after the QT's UI holding the instance of this widget is initialized.
	/// @param bar Model responsible for game logic.
	void initializeConnections(BarModel *bar);

public slots:
	/// @brief Displays the given QPixmap of liquid.
	/// @param liquid New QPixmap to display.
	void updateLiquidDisplay(const QPixmap &liquid);

	/// @brief Stops displaying the last image of liquid.
	void removeLiquidDisplay();

	/// @brief Displays the given QPixmap of glassware.
	/// @param glassware Contains new QPixmap to display.
	void setNewGlassware(const Glassware &glassware);

	/// @brief Stops displaying the glassware.
	void removeGlasswareDisplay();

private:
	static constexpr int WIDTH = 256;
	static constexpr int HEIGHT = 280;

	QGraphicsScene gScene;
	QGraphicsItemGroup gItemGroup;

	QGraphicsPixmapItem gMug;
	QGraphicsPixmapItem gLiquid;
	QGraphicsPixmapItem gGlass;

	QPixmap emptyPixmap;

};

#endif // DRINKVIEW_H
