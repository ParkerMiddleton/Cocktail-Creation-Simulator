#ifndef BARMODEL_H
#define BARMODEL_H

#include "recipe.h"

#include <QObject>
#include <QTimer>

#include "liquidmodel.h"

/// @brief Responsible for game logic. Part of Model.
class BarModel : public QObject
{
	Q_OBJECT
public:
	/// @brief Constructor.
	explicit BarModel(QObject *parent = nullptr);

	void pause();
	void unpause();

	LiquidModel* liquidModel();

public slots:
	// void bourbonPressed();
	// void bittersPressed();
	// void vodkaPressed();
	// void collinsPressed();
	// void copperMugPressed();

	void ingredientPressed(const QString &liqourName);
	void processLiquor();
	void ingredientReleased();
	void ingredientClicked(const QString &ingredientName);
	void serveDrink();
	void emptyDrink();
	void restartGame();

signals:
	void newDrink(const QString &recipe);
    void sendVolume(int v);
    void sendDrinkName(const QString ingredient);
    void informEmptyDrink();

private:
	LiquidModel liquid;

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

    void newRound();
    void newGame();
    void getRandomRecipe();

};

#endif // BARMODEL_H
