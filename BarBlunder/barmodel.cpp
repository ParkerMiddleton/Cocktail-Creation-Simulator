#include "barmodel.h"
#include "recipe.h"

#include <QFile>
#include <QDebug>
#include <QRandomGenerator>

BarModel::BarModel(QObject *parent)
	: QObject{parent}
	, rocksGlass{Glassware::Type::Rocks}
	, collinsGlass{Glassware::Type::Collins}
	, copperMug{Glassware::Type::Copper}
	, martiniGlass{Glassware::Type::Martini}
    , rocksGlassOrange{Glassware::Type::RocksOrange}
    , rocksGlassLime{Glassware::Type::RocksLime}
    , rocksGlassOlives{Glassware::Type::RocksOlives}
    , collinsGlassLime{Glassware::Type::CollinsLime}
    , collinsGlassOrange{Glassware::Type::CollinsOrange}
    , collinsGlassOlives{Glassware::Type::CollinsOlives}
    , copperMugLime{Glassware::Type::CopperLime}
    , copperMugOlive{Glassware::Type::CopperOlives}
    , copperMugOrange{Glassware::Type::CopperOrange}
    , martiniGlassOrange{Glassware::Type::MartiniOrange}
    , martiniGlassLime{Glassware::Type::MartiniLime}
    , martiniGlassOlives{Glassware::Type::MartiniOlives}
	, isGlasswarePlaced{false}
	, isGlasswareEmpty{true}
{
	timer.setInterval(1000); // Set interval to 1000 milliseconds (1 second)
	connect(&timer, &QTimer::timeout, this, &BarModel::processLiquor);

	QFile file(":/text/drinkingredients.txt");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Failed to open file from resources";
		return;
	}

	QTextStream inStream(&file);
	while (!inStream.atEnd())
	{
		listOfRecipes.push_back(Recipe(inStream));
	}
}

void BarModel::startNewGame()
{
	this->emptyDrink();
	this->removeGlassware();
	this->getRandomRecipe();
	score = 0;

	qDebug() << "------------- NEW GAME -------------------------";
}

void BarModel::setIsPaused(bool state)
{
	liquid.setIsSimulationPaused(state);
}

LiquidModel* BarModel::liquidModel()
{
	return &liquid;
}

void BarModel::ingredientPressed(const QString &liquorName)
{

	liquorSelection = liquorName;
	pressedLiquor = true;
	liquid.updateDrinkColor(liquorSelection);


	// dont start timer if clicked
	// Start a single-shot timer with a delay of 1000 milliseconds (1 second)
	timer.start();
}
void BarModel::processLiquor()
{
    // TODO:: Review why pressing wrong liquor decrements it by double the amount.
    if (!isGlasswarePlaced)
        return;

    if (isGlasswareEmpty)
        isGlasswareEmpty = false;

    bool found = false;
    QPair<QString, int> &ingredient = userRecipe.ingredients[stepNumber];

    if (ingredient.first == liquorSelection)
    {
        found = true;
    }

    if (!found)
    {
        qDebug() << "wrong ingredient pressed" << liquorSelection;
        outOfOrder = true;
        userRecipe.ingredients.push_back(QPair<QString, int>(liquorSelection, -1));
        emit incorrectIngredientUsed(stepNumber);
    }

    if (pressedLiquor)
    { // Check if the whiskey button is still pressed
        for (QPair<QString, int> &ingredient : userRecipe.ingredients)
        {
            if (ingredient.first == liquorSelection)
            {
                ingredient.second--;
                volume++;
                if(liquorSelection != "shake"){
                    liquid.addLiquid(1);
                }
                qDebug() << liquorSelection << " " << ingredient.second;
                if (ingredient.second == 0)
                {
                    stepNumber++;
                    qDebug() << "Step Number:" << stepNumber;
                    emit correctIngredientUsed(stepNumber);
                }
                else if(ingredient.second < 0){
                    stepNumber++;
                    qDebug() << "Step Number:" << stepNumber;
                    emit incorrectIngredientUsed(stepNumber);
                }
            }

        }
    }
    // allows to successfully pour but cant clear all particles
}
void BarModel::ingredientReleased()
{
	if (!isGlasswarePlaced)
		return;

	// short term solution for clearing old liquid allowing for the particles to be cleared
	//emit informEmptyDrink();
	// allows for successful removal of all particles but cant pour while holding down
	//emit sendVolume(volume);
	pressedLiquor = false;
	timer.stop();
	volume = 0;
}

void BarModel::ingredientClicked(const QString &ingredientName)
{
	// TEMP :: most likely buggy :: BETTER SOLUTION INCOMING (hopefully)
	if (!isGlasswarePlaced || isGlasswareEmpty)
	{
		if (ingredientName == "rocks glass")
		{
			isGlasswarePlaced = true;
            currentGlassware = "rocks glass";
			emit glasswareUpdated(rocksGlass);
			liquid.updateCollisionLayout(rocksGlass);
		}
		else if (ingredientName == "collins glass")
		{
			isGlasswarePlaced = true;
            currentGlassware = "collins glass";
			emit glasswareUpdated(collinsGlass);
			liquid.updateCollisionLayout(collinsGlass);
		}
		else if (ingredientName == "copper mug")
		{
			isGlasswarePlaced = true;
            currentGlassware = "copper mug";
			emit glasswareUpdated(copperMug);
			liquid.updateCollisionLayout(copperMug);
		}
		else if (ingredientName == "martini glass")
		{
			isGlasswarePlaced = true;
            currentGlassware = "martini glass";
			emit glasswareUpdated(martiniGlass);
			liquid.updateCollisionLayout(martiniGlass);
		}
		else if (!isGlasswarePlaced)
		{
			return;
		}
	}

	if (ingredientName == "ice")
	{
		liquid.addIce();
	}

    if(ingredientName == "orange peele" && currentGlassware == "rocks glass"){
        emit glasswareUpdated(rocksGlassOrange);
    }
    if(ingredientName == "lime wedge" && currentGlassware == "rocks glass"){
        emit glasswareUpdated(rocksGlassLime);
    }
    if(ingredientName == "olives" && currentGlassware == "rocks glass"){
        emit glasswareUpdated(rocksGlassOlives);
    }


    if(ingredientName == "orange peele" && currentGlassware == "collins glass"){
        emit glasswareUpdated(collinsGlassOrange);
    }
    if(ingredientName == "lime wedge" && currentGlassware == "collins glass"){
        emit glasswareUpdated(collinsGlassLime);
    }
    if(ingredientName == "olives" && currentGlassware == "collins glass"){
        emit glasswareUpdated(collinsGlassOlives);
    }

    if(ingredientName == "orange peele" && currentGlassware == "copper mug"){
        emit glasswareUpdated(copperMugOrange);
    }
    if(ingredientName == "lime wedge" && currentGlassware == "copper mug"){
        emit glasswareUpdated(copperMugLime);
    }
    if(ingredientName == "olives" && currentGlassware == "copper mug"){
        emit glasswareUpdated(copperMugOlive);
    }

    if(ingredientName == "orange peele" && currentGlassware == "martini glass"){
        emit glasswareUpdated(martiniGlassOrange);
    }
    if(ingredientName == "lime wedge" && currentGlassware == "martini glass"){
        emit glasswareUpdated(martiniGlassLime);
    }
    if(ingredientName == "olives" && currentGlassware == "martini glass"){
        emit glasswareUpdated(martiniGlassOlives);
    }

	QPair<QString, int> &ingredient = userRecipe.ingredients[stepNumber];

	if (ingredient.first == ingredientName)
	{
		if (isGlasswareEmpty)
			isGlasswareEmpty = false;

		ingredient.second--;
		qDebug() << ingredientName << " " << ingredient.second;

		if (ingredient.second == 0)
			stepNumber++;

		qDebug() << stepNumber;

		emit correctIngredientUsed(stepNumber);
	}
	else
	{
		qDebug() << "wrong ingredient clicked" << ingredientName;
		userRecipe.ingredients.push_back(QPair<QString, int>(ingredientName, -1));

		emit incorrectIngredientUsed(stepNumber);
	}
}

void BarModel::serveDrink()
{
	// check recipe validity
	bool correctDrink = userRecipe.checkServedDrink(assignedRecipe);

	// if it is correct call new round without resetting game
	if (correctDrink && !outOfOrder)
	{
		emit drinkIsCorrect();
		qDebug() << "Congratulations! you made the drink correct";
		score++;
		this->startNewRound();
	}
	else
	{
		emit drinkIsIncorrect();
		qDebug() << "Drink ingredients is out of order: " << outOfOrder;
		qDebug() << "Correct drink portions: " << correctDrink;
		this->startNewRound();
	}
	// else maybe give a error that they made the drink incorrect and start new game or retry that recipe
}

void BarModel::emptyDrink()
{
	// reassign user drink to assigned recipe effectivly emptying the drink.
	isGlasswareEmpty = true;
	userRecipe = assignedRecipe;
	stepNumber = 0;
	volume = 0;
	outOfOrder = false;

	liquid.clear();

	// inform view to empty drink
	emit drinkEmptied();

	qDebug() << "drink emptied";
}



void BarModel::startNewRound()
{
	this->emptyDrink();
	this->removeGlassware();

	this->getRandomRecipe();

	qDebug() << "------------- NEW ROUND -------------------------";
}

void BarModel::getRandomRecipe()
{
	// duplicate recipes possible in this case currently.
	int randomNumber = QRandomGenerator::global()->bounded(listOfRecipes.size());

	while (true)
	{
		if (assignedRecipe.drinkName != listOfRecipes[randomNumber].drinkName)
		{
			break;
		}
		else
		{
			randomNumber = QRandomGenerator::global()->bounded(listOfRecipes.size());
		}
	}

    assignedRecipe = listOfRecipes[4]; // debug purposes
	userRecipe = assignedRecipe;

	//assignedRecipe = listOfRecipes[randomNumber];
	emit newDrink(assignedRecipe.recipeSteps);
}

void BarModel::removeGlassware()
{
	isGlasswarePlaced = false;
	liquid.removeCollisionLayout();
	emit glasswareRemoved();
}

