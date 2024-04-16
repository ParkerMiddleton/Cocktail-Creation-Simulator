#ifndef BARMODEL_H
#define BARMODEL_H

#include "recipe.h"

#include <QObject>
#include <QTimer>

/// @brief Responsible for game logic. Part of Model.
class BarModel : public QObject
{
	Q_OBJECT
public:
	/// @brief Constructor.
	explicit BarModel(QObject *parent = nullptr);

	void pause();
	void unpause();

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

private:
	QList<Recipe> listOfRecipes;
	Recipe assignedRecipe;
	Recipe userRecipe;
	int stepNumber;
	int score;
	bool outOfOrder;
	bool pressedLiquor;
	QTimer timer;
	QString liquorSelection;

	void newRound();
	void newGame();
	void getRandomRecipe();

};

#endif // BARMODEL_H
