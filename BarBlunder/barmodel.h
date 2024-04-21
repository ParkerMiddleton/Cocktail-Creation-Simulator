#ifndef BARMODEL_H
#define BARMODEL_H

#include "recipe.h"
#include "liquidmodel.h"
#include "glassware.h"

#include <QObject>
#include <QTimer>

/// @brief Responsible for game logic. Part of Model.
class BarModel : public QObject
{
	Q_OBJECT
public:
	/// @brief Constructor.
	explicit BarModel(QObject *parent = nullptr);

	void startNewGame();
	void setIsPaused(bool state);

	LiquidModel* liquidModel();

public slots:
	// void bourbonPressed();
	// void bittersPressed();
	// void vodkaPressed();
	// void collinsPressed();
	// void copperMugPressed();

	void ingredientPressed(const QString &liqourName);
	void ingredientReleased();
	void ingredientClicked(const QString &ingredientName);

	void serveDrink();
	void emptyDrink();

signals:
	void newDrink(const QList<QString> &recipeSteps);
	void drinkEmptied();

	void glasswareUpdated(const Glassware &glassware);
	void glasswareRemoved();

	void correctIngredientUsed(int stepNumber);
	void incorrectIngredientUsed(int stepNumber);

	void drinkIsCorrect();
	void drinkIsIncorrect();

private slots:
	void processLiquor();

private:
	LiquidModel liquid;
	Glassware rocksGlass;
	Glassware collinsGlass;
	Glassware copperMug;
	Glassware martiniGlass;

	bool isGlasswarePlaced;
	bool isGlasswareEmpty;

    QList<Recipe> listOfRecipes;
    Recipe assignedRecipe;
    Recipe userRecipe;
    int stepNumber;
    int score;
    int volume;
    bool outOfOrder;
    bool pressedLiquor;
    bool doublePour;
    QTimer timer;
	QString liquorSelection;

	void startNewRound();
    void getRandomRecipe();
	void removeGlassware();

};

#endif // BARMODEL_H
