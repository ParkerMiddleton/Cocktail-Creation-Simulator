#include "barmodel.h"
#include "recipe.h"

#include <QFile>
#include <QDebug>
#include <QRandomGenerator>

BarModel::BarModel(QObject *parent)
	: QObject{parent}
	, currentGlassware{nullptr}
	, isGlasswareEmpty{true}

	, recipeStepNumber{0}
	, currentProcessingIngredient{""}
	, score{0}

	, isProcessingIngredient{false}
	, processingElapsedTime{0}
	, processingElapsedTimeTotal{0}
	, isShaking{false}
	, isOutOfOrder{false}
{
	glasswares.insert("rocks glass", new Glassware{Glassware::Type::Rocks});
	glasswares.insert("collins glass", new Glassware{Glassware::Type::Collins});
	glasswares.insert("copper mug", new Glassware{Glassware::Type::Copper});
	glasswares.insert("martini glass", new Glassware{Glassware::Type::Martini});

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

BarModel::~BarModel()
{
	for (auto iter = glasswares.begin(); iter != glasswares.end(); ++iter)
	{
		delete iter.value();
	}
}

LiquidModel* BarModel::liquidModel()
{
	return &liquid;
}

void BarModel::startNewGame()
{
	qDebug() << "------------- NEW GAME -------------------------";
	score = 0;
	this->startNewRound();
}

void BarModel::update(int deltaTime)
{
	if (isProcessingIngredient)
	{
		if (processingElapsedTime >= 1000) // Process every 1000 milliseconds
		{
			processingElapsedTimeTotal += processingElapsedTime;
			emit totalProcessingTimerUpdated(qRound(((float)processingElapsedTimeTotal) / 1000.0f)); // Round to the nearest second and notify.

			processingElapsedTime = -deltaTime; // Reset.

			this->processPressedIngredient(); // Update processing.
		}

		processingElapsedTime += deltaTime;
	}

	if (!isShaking)
	{
		liquid.update(deltaTime);
	}
}

void BarModel::ingredientPressed(const QString &ingredientName)
{
	if (recipeStepNumber >= userRecipe.ingredients.size()) // If recipe is complete.
		return;

	if (!currentGlassware) // If no glassware is placed
		return;

	if (isGlasswareEmpty && !glasswares.contains(ingredientName)) // If ingredient is not one of the glasses.
		isGlasswareEmpty  = false;

	currentProcessingIngredient = ingredientName;

	processingElapsedTime = 0;
	processingElapsedTimeTotal = 0;
	isProcessingIngredient = true;

	if (currentProcessingIngredient == "shake")
	{
		isShaking = true;
		emit shaking();
	}
}

void BarModel::ingredientReleased()
{
	if (recipeStepNumber >= userRecipe.ingredients.size()) // If recipe is complete.
		return;

	if (!currentGlassware)
		return;

	isProcessingIngredient = false;

	if (currentProcessingIngredient == "shake")
	{
		isShaking = false;
		emit stopShaking();
	}

	emit totalProcessingTimerUpdated(0);
}

void BarModel::ingredientClicked(const QString &ingredientName)
{
	if (recipeStepNumber >= userRecipe.ingredients.size()) // If recipe is complete.
		return;

	// Disable all ingredient click functionality until glass is selected
	if (!currentGlassware || isGlasswareEmpty)
	{
		if (glasswares.contains(ingredientName))
		{
			currentGlassware = glasswares.value(ingredientName);
			emit glasswareUpdated(*currentGlassware);
			liquid.updateGlassware(*currentGlassware);
		}
		else if (!currentGlassware)
		{
			return;
		}
	}

	if (ingredientName == "olives")
	{
		currentGlassware->placeGarnish(Glassware::Garnish::Olive);
		emit glasswareUpdated(*currentGlassware);
	}
	else if (ingredientName == "lime wedge")
	{
		currentGlassware->placeGarnish(Glassware::Garnish::Lime);
		emit glasswareUpdated(*currentGlassware);
	}
	else if (ingredientName == "orange peel")
	{
		currentGlassware->placeGarnish(Glassware::Garnish::Orange);
		emit glasswareUpdated(*currentGlassware);
	}
	else if (ingredientName == "ice")
	{
		liquid.addIce();
	}
	else if (ingredientName == "stir")
	{
		liquid.mix();
	}
	else if (ingredientName == "bitters" || ingredientName == "orange liquor splash")
	{
		liquid.dash(ingredientName);
	}

	QPair<QString, int> &ingredient = userRecipe.ingredients[recipeStepNumber];

	if (isGlasswareEmpty && !glasswares.contains(ingredientName)) // If ingredient is not one of the glasses.
		isGlasswareEmpty  = false;

	if (ingredient.first == ingredientName)
	{
		ingredient.second--;
		qDebug() << ingredientName << " " << ingredient.second;

		if (ingredient.second == 0)
			recipeStepNumber++;

		qDebug() << recipeStepNumber;

		emit correctIngredientUsed(recipeStepNumber);
		emit correctIngredientName(ingredientName);
	}
	else
	{
		qDebug() << "wrong ingredient clicked" << ingredientName;
		userRecipe.ingredients.push_back(QPair<QString, int>(ingredientName, -1));

		emit incorrectIngredientUsed(recipeStepNumber);
	}
}

void BarModel::serveDrink()
{
	// check recipe validity
	bool correctDrink = userRecipe.checkServedDrink(assignedRecipe);

	// if it is correct call new round without resetting game
	if (correctDrink && !isOutOfOrder)
	{
		emit drinkIsCorrect();
		qDebug() << "Congratulations! you made the drink correct";
		score++;
		this->startNewRound();
	}
	else
	{
		emit drinkIsIncorrect();
		qDebug() << "Drink ingredients is out of order: " << isOutOfOrder;
		qDebug() << "Correct drink portions: " << correctDrink;
		this->startNewRound();
	}
	// else maybe give a error that they made the drink incorrect and start new game or retry that recipe
}

void BarModel::emptyDrink()
{
	// reassign user drink to assigned recipe effectivly emptying the drink.
	isGlasswareEmpty = true;
	userRecipe = assignedRecipe; // doesnt this need to restart the recipe?
	recipeStepNumber = 0;
	isOutOfOrder = false;

	liquid.empty();

	for (auto iter = glasswares.begin(); iter != glasswares.end(); ++iter)
	{
		iter.value()->placeGarnish(Glassware::Garnish::None);
	}

	if (currentGlassware)
		emit glasswareUpdated(*currentGlassware);

	// inform view to empty drink
	emit drinkEmptied();
	emit newDrink(assignedRecipe.steps);
	this->removeGlassware();
	qDebug() << "drink emptied";
}

void BarModel::processPressedIngredient()
{
	if (recipeStepNumber >= userRecipe.ingredients.size()) // If recipe is complete.
		return;

	// TODO:: Review why pressing wrong liquor decrements it by double the amount. // FIXED?
	bool found = false;

	QPair<QString, int> &ingredient = userRecipe.ingredients[recipeStepNumber];

	if (ingredient.first == currentProcessingIngredient)
	{
		found = true;
	}

	if (!found)
	{
		if (currentProcessingIngredient != "shake")
		{
			liquid.pour(1, currentProcessingIngredient);
		}

		qDebug() << "wrong ingredient pressed" << currentProcessingIngredient;
		isOutOfOrder = true;
		userRecipe.ingredients.push_back(QPair<QString, int>(currentProcessingIngredient, -1));
		emit incorrectIngredientUsed(recipeStepNumber);
	}
	else if (isProcessingIngredient)
	{
		for (QPair<QString, int> &ingredient : userRecipe.ingredients)
		{
			if (ingredient.first == currentProcessingIngredient)
			{
				ingredient.second--;

				if (currentProcessingIngredient != "shake")
				{
					liquid.pour(1, currentProcessingIngredient);
				}

				qDebug() << currentProcessingIngredient << " " << ingredient.second;

				if (ingredient.second == 0)
				{
					recipeStepNumber++;
					qDebug() << "Step Number 1:" << recipeStepNumber;
					emit correctIngredientUsed(recipeStepNumber);
					emit correctIngredientName(ingredient.first);

					if (currentProcessingIngredient == "shake")
						liquid.mix();
				}
				else if (ingredient.second < 0)
				{
					recipeStepNumber++;
					qDebug() << "Step Number 2:" << recipeStepNumber;
					emit incorrectIngredientUsed(recipeStepNumber);
					emit correctIngredientName(ingredient.first);

					if (currentProcessingIngredient == "shake")
						liquid.mix();
				}
			}
		}
	}
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

	assignedRecipe = listOfRecipes[randomNumber];
	userRecipe = assignedRecipe;

	emit newDrink(assignedRecipe.steps);
	emit drinkOrder(assignedRecipe.drinkName);
}

void BarModel::removeGlassware()
{
	currentGlassware = nullptr;
	liquid.removeGlassware();
	emit currentGlasswareRemoved();
}
