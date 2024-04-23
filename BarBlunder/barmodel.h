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
	explicit BarModel(QObject *parent = nullptr);

	~BarModel();

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

    void updateGlassClicked();

signals:
	void newDrink(const QList<QString> &recipeSteps);
	void drinkEmptied();

	/// @brief glasswareUpdated this signal updates the glassware by calling a specific type
	/// @param glassware the specific form of glassware being called
	void glasswareUpdated(const Glassware &glassware);

	void shaking();

	/// @brief this signal removes glassware
	void currentGlasswareRemoved();

	void correctIngredientUsed(int stepNumber);
	void incorrectIngredientUsed(int stepNumber);

	void drinkIsCorrect();
	void drinkIsIncorrect();

    void elapsedTimePressed(int time);

private slots:
	/// @brief this slot process the the liquor
	void processLiquor();

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

	/// @brief starts a new round
	void startNewRound();
	/// @brief this method retrives a random recipe
	void getRandomRecipe();
	/// @brief this method removes the glassware
	void removeGlassware();

    QElapsedTimer elapsedTimer;
    QTimer pressTimer;

};

#endif // BARMODEL_H
