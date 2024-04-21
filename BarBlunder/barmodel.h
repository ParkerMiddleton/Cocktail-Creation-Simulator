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

    /// @brief starts game
	void startNewGame();
    /**
     * @brief setIsPaused pauses the game
     * @param state-state if paused
     */
	void setIsPaused(bool state);

    /// @brief model of the liquid system
	LiquidModel* liquidModel();

public slots:
    /**
     * @brief ingredientPressed - slot to handle pressing an ingredient
     * @param liqourName -- the string representing that specific ingredient as a qstring
     */
	void ingredientPressed(const QString &liqourName);
    /// @brief slot to handle releasing hte ingredient list
	void ingredientReleased();
    /**
     * @brief ingredientClicked-slot to handle pressing an ingredient
     * @param ingredientName- the name of the ingredient as a qstring
     */
	void ingredientClicked(const QString &ingredientName);

    /// @brief this slot serves a drink
	void serveDrink();
    /// @brief this slot emties drinks
	void emptyDrink();
    /// @brief this slot hides drinks
    void hideDrink();
    /// @brief this slot exposes the drink
    void exposeDrink();

signals:
    /**
     * @brief newDrink this signal calls a new drink
     * @param recipe
     */
	void newDrink(const QString &recipe);
    /// @brief this signal empties the drink
	void drinkEmptied();
    /**
     * @brief glasswareUpdated this signal updates the glassware by calling a specific type
     * @param glassware the specific form of glassware being called
     */
	void glasswareUpdated(const Glassware &glassware);
    /// @brief this signal removes glassware
	void glasswareRemoved();

private slots:
    /// @brief this slot process the the liquor
	void processLiquor();

private:
    /// @brief the model for the liquid
	LiquidModel liquid;
    /// @brief this is a rocks glass
	Glassware rocksGlass;
    /// @brief this is a collins glass
	Glassware collinsGlass;
    /// @brief literally a copper mug
	Glassware copperMug;
    /// @brief a martini glass
	Glassware martiniGlass;
    /**
     *  @brief if the glassware has been placed or not
     *  @return if glass ware is placed or not
     */
	bool isGlasswarePlaced;
    /**
     * @brief if the glassware is empty or not
     * @return if glassware is empty or not
     */
	bool isGlasswareEmpty;
    /// @brief a QList full of Recipe objects that is used to curate the recipes avalible in game
    QList<Recipe> listOfRecipes;
    /// @brief the assigned recipe for a round of the game
    Recipe assignedRecipe;
    /// @brief the user's creation as a recipe object
    Recipe userRecipe;
    /// @brief the step of recipe as a int
    int stepNumber;
    /// @brief the score the user gets
    int score;
    /// @brief the volume of the music
    int volume;
    /// @brief if the stuff is in order or not
    bool outOfOrder;
    /// @brief if the bottle is pressed
    bool pressedLiquor;
    /// @brief if double pour happened
    bool doublePour;
    /// @brief a time that times the round
    QTimer timer;
    /// @brief QString representing the liquor selected
	QString liquorSelection;
    /// @brief starts a new round
	void startNewRound();
    /// @brief this method retrives a random recipe
    void getRandomRecipe();
    /// @brief this method removes the glassware
	void removeGlassware();

};

#endif // BARMODEL_H
