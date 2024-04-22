#ifndef BARMODEL_H
#define BARMODEL_H

#include "recipe.h"
#include "liquidmodel.h"
#include "glassware.h"

#include <QObject>

/// @brief Responsible for game logic. Part of Model.
class BarModel : public QObject
{
	Q_OBJECT

public:
	/// @brief Constructor.
	explicit BarModel(QObject *parent = nullptr);

	/// @brief starts game
	void startNewGame();

	void update(int deltaTime);

	/// @brief model of the liquid system
	LiquidModel* liquidModel();

public slots:
	/// @brief ingredientPressed - slot to handle pressing an ingredient
	/// @param liqourName -- the string representing that specific ingredient as a qstring
	void ingredientPressed(const QString &liqourName);

	/// @brief slot to handle releasing hte ingredient list
	void ingredientReleased();

	/// @brief ingredientClicked-slot to handle pressing an ingredient
	/// @param ingredientName- the name of the ingredient as a qstring
	void ingredientClicked(const QString &ingredientName);

	/// @brief this slot serves a drink
	void serveDrink();

	/// @brief this slot emties drinks
	void emptyDrink();

signals:
	void newDrink(const QList<QString> &recipeSteps);
	void drinkEmptied();

	/// @brief glasswareUpdated this signal updates the glassware by calling a specific type
	/// @param glassware the specific form of glassware being called
	void glasswareUpdated(const Glassware &glassware);

	/// @brief this signal removes glassware
	void glasswareRemoved();

	void correctIngredientUsed(int stepNumber);
	void incorrectIngredientUsed(int stepNumber);

	void drinkIsCorrect();
	void drinkIsIncorrect();

private slots:
	/// @brief this slot process the the liquor
	void processLiquor();

private:
	LiquidModel liquid;
	Glassware rocksGlass;
	Glassware collinsGlass;
	Glassware copperMug;
	Glassware martiniGlass;
    Glassware rocksGlassOrange;
    Glassware rocksGlassLime;
    Glassware rocksGlassOlives;
    Glassware collinsGlassLime;
    Glassware collinsGlassOrange;
    Glassware collinsGlassOlives;
    Glassware copperMugLime;
    Glassware copperMugOlive;
    Glassware copperMugOrange;
    Glassware martiniGlassOrange;
    Glassware martiniGlassLime;
    Glassware martiniGlassOlives;


	bool isGlasswarePlaced;
	bool isGlasswareEmpty;

	bool isProcessing;
	int processingElapsedTime;

	QList<Recipe> listOfRecipes;
	Recipe assignedRecipe;
	Recipe userRecipe;
	int stepNumber;
	int score;
	int volume;
	bool outOfOrder;
	bool pressedLiquor;
	bool doublePour;
	QString liquorSelection;

    QString currentGlassware;

	/// @brief starts a new round
	void startNewRound();
	/// @brief this method retrives a random recipe
	void getRandomRecipe();
	/// @brief this method removes the glassware
	void removeGlassware();

};

#endif // BARMODEL_H
