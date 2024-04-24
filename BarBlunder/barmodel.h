#ifndef BARMODEL_H
#define BARMODEL_H

#include "recipe.h"
#include "liquidmodel.h"
#include "glassware.h"

#include <QObject>
#include <QMap>
#include <QElapsedTimer>
#include <QTimer>

/// @brief Responsible for game logic. Part of Model.
/// @extends QObject
class BarModel : public QObject
{
	Q_OBJECT

public:
	/// @brief Constructor.
	/// Loads file with all of the ingredients.
	explicit BarModel(QObject *parent = nullptr);

	/// @brief Destructor.
	~BarModel();

	/// @brief Returns a pointer to the LiquidModel.
	/// @return LiquidModel*
	LiquidModel* liquidModel();

	/// @brief Starts a new game.
	void startNewGame();

	/// @brief Updates the game logic, and controls the update method of LiquidModel.
	/// @param deltaTime Time between the last and current update.
	void update(int deltaTime);

public slots:
	/// @brief Sets the given ingredient as pressed.
	/// Meaning it will be processed until the player releases it.
	/// @param ingredientName Ingredient to set as pressed.
	void ingredientPressed(const QString &ingredientName);

	/// @brief Releases the current pressed ingredient.
	/// Meaning the processing of it will stop.
	void ingredientReleased();

	/// @brief Processes the given ingredient one time.
	/// @param ingredientName Ingredient to process.
	void ingredientClicked(const QString &ingredientName);

	/// @brief Serves the drink made by player and starts a new round.
	void serveDrink();

	/// @brief Empties the drink and removes the glassware.
	void emptyDrink();

	// TODO: REFACTOR!!!
	/// @brief Places the glassware.
	void updateGlassClicked();

signals:
	/// @brief Notifies that the new recipe steps were assigned to make.
	/// @param recipeSteps List of recipe steps.
	void newDrink(const QList<QString> &recipeSteps);

	/// @brief Notifies that the drink was emptied.
	void drinkEmptied();

	/// @brief Notifies that either the new glassware was placed or garnishes were placed on the current one.
	void glasswareUpdated(const Glassware &glassware);

	/// @brief Notifies that shaker tins are currently in use.
	void shaking();

	/// @brief Notifies that the places glassware was removed.
	void currentGlasswareRemoved();

	/// @brief Notifies that the current step was done correctly.
	/// @param recipeStepNumber Recipe's step number.
	void correctIngredientUsed(int recipeStepNumber);

	/// @brief Notifies that the current step was done incorrectly.
	/// @param recipeStepNumber Recipe's step number.
	void incorrectIngredientUsed(int recipeStepNumber);

	/// @brief Notifies the name of the correct ingredient used during the current step.
	/// @param currentIngredient Correct ingredient.
	void correctIngredientName(const QString &currentIngredient);

	/// @brief Notifes that the recipe was completed correctly.
	void drinkIsCorrect();

	/// @brief Notifes that the recipe was completed incorrectly.
	void drinkIsIncorrect();

	/// @brief Notifies the name of the drink to make.
	/// @param drinkName Current recipe's drink name.
	void drinkOrder(const QString &drinkName);

	// TODO: REFACTOR?
	void elapsedTimePressed(int time);

private slots:
	// REFACTOR?
	void updatePressedTimer();

private:
	LiquidModel liquid;
	QMap<QString, Glassware*> glasswares;
	Glassware *currentGlassware;
	bool isGlasswareEmpty;

	bool isProcessing;
	int processingElapsedTime;
	bool isShaking;

	QList<Recipe> listOfRecipes;
	Recipe assignedRecipe;
	Recipe userRecipe;
	int stepNumber;
	int score;
	bool outOfOrder;
	bool pressedLiquor;

	QString currentLiquor;

	bool glasswareClicked;

	QElapsedTimer elapsedTimer;
	QTimer pressTimer;

	/// @brief Processes currently pressed ingredient.
	/// Currently runs every 1000 milliseconds from the update method.
	void processPressedIngredient();

	/// @brief Starts a new round.
	void startNewRound();

	/// @brief Assigns a new random recipe to follow.
	void getRandomRecipe();

	/// @brief Removes the currently placed glassware and emits appropriate signal.
	void removeGlassware();

};

#endif // BARMODEL_H
