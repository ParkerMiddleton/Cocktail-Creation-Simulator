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

	/// @brief Used to emit any signals to the view before the application is fully started, if needed.
	/// Reason why it exists:
	/// according to the official QT documentation and various online sources,
	/// any signal emitted inside the constructor does not go to its respective slot.
	void initialize();

	void pause();
	void unpause();

public slots:

    // void bourbonPressed();
    // void bittersPressed();
    // void vodkaPressed();
    // void collinsPressed();
    // void copperMugPressed();

    void ingredientPressed(QString liqourName);
    void processLiquor();
    void ingredientReleased();
    void ingredientClicked(QString ingreidentName);
    void serveDrink();
    void emptyDrink();
    void restartGame();

signals:
    void newDrink(QString recipe);

private:
    QList<recipe> listOfRecipes;
    recipe assignedRecipe;
    recipe userRecipe;
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
