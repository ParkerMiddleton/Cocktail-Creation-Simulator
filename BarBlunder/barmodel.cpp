#include "barmodel.h"
#include "recipe.h"

#include <QFile>
#include <QDebug>
#include <QRandomGenerator>

BarModel::BarModel(QObject *parent)
	: QObject{parent}
	, currentGlassware{nullptr}
	, isGlasswareEmpty{true}
	, isProcessing{false}
	, processingElapsedTime{0}
	, processingElapsedTimeTotal{0}
	, isShaking{false}
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
	if (isProcessing)
	{
		if (processingElapsedTime >= 1000) // Process every 1000 milliseconds
		{
			processingElapsedTimeTotal += processingElapsedTime;
			emit totalProcessingTimerUpdated(qRound(((float)processingElapsedTimeTotal) / 1000.0f)); // Round to the nearest second and notify.

			processingElapsedTime = -deltaTime; // Reset.

			// dont process liquor unless glass is clicked
			if (glasswareClicked)
			{
				this->processPressedIngredient();
			}
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
	if(!glasswareClicked) {
		return;
	}

	currentLiquor = ingredientName;

	processingElapsedTime = 0;
	processingElapsedTimeTotal = 0;
	isProcessing = true;
	pressedLiquor = true;

	if (currentLiquor == "shake")
	{
		isShaking = true;
		emit shaking();
	}
}

void BarModel::ingredientReleased()
{
	if (!currentGlassware)
		return;

	emit totalProcessingTimerUpdated(0);

	pressedLiquor = false;
	isProcessing = false;

	if (currentLiquor == "shake")
		isShaking = false;
}

void BarModel::ingredientClicked(const QString &ingredientName)
{
	// disable all ingredient click functionality until glass selected
	if(!glasswareClicked && (ingredientName != "rocks glass" || ingredientName != "martini glass" || ingredientName != "collins glass" || ingredientName != "copper mug")) {
		return;
	}

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
		emit correctIngredientName(ingredientName);
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
	userRecipe = assignedRecipe; // doesnt this need to restart the recipe?
	stepNumber = 0;
	outOfOrder = false;

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

void BarModel::updateGlassClicked()
{
	glasswareClicked = true;
}

void BarModel::processPressedIngredient()
{
	// TODO:: Review why pressing wrong liquor decrements it by double the amount.
	if (!currentGlassware)
		return;

	if (isGlasswareEmpty)
		isGlasswareEmpty = false;

	bool found = false;

	QPair<QString, int> &ingredient = userRecipe.ingredients[stepNumber];

	if (ingredient.first == currentLiquor)
	{
		found = true;
	}

	if (!found)
	{
		liquid.pour(1, currentLiquor);
		qDebug() << "wrong ingredient pressed" << currentLiquor;
		outOfOrder = true;
		userRecipe.ingredients.push_back(QPair<QString, int>(currentLiquor, -1));
		emit incorrectIngredientUsed(stepNumber);
	}
	else if (pressedLiquor)
	{ // Check if the whiskey button is still pressed
		for (QPair<QString, int> &ingredient : userRecipe.ingredients)
		{
			if (ingredient.first == currentLiquor)
			{
				ingredient.second--;

				if (currentLiquor != "shake")
				{
					liquid.pour(1, currentLiquor);
				}

				qDebug() << currentLiquor << " " << ingredient.second;

				if (ingredient.second == 0)
				{
					stepNumber++;
					qDebug() << "Step Number 1:" << stepNumber;
					emit correctIngredientUsed(stepNumber);
					emit correctIngredientName(ingredient.first);

					if (currentLiquor == "shake")
						liquid.mix();
				}
				else if(ingredient.second < 0)
				{
					stepNumber++;
					qDebug() << "Step Number 2:" << stepNumber;
					emit incorrectIngredientUsed(stepNumber);
					emit correctIngredientName(ingredient.first);

					if (currentLiquor == "shake")
						liquid.mix();
				}
			}
		}
	}
}

void BarModel::startNewRound()
{
	glasswareClicked = false;
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

	assignedRecipe = listOfRecipes[randomNumber]; // randomNumber
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
