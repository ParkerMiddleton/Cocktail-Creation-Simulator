#include "barmodel.h"
#include "recipe.h"

BarModel::BarModel(QObject *parent)
	: QObject{parent}
{
    timer.setInterval(1000); // Set interval to 1000 milliseconds (1 second)
    connect(&timer, &QTimer::timeout, this, &BarModel::processLiquor);
}

void BarModel::initialize()
{
    QFile file(":/textfiles/drinkingredients.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file from resources";
        return;
    }

    QTextStream* in = new QTextStream(&file);
    while (!in->atEnd()) {
        listOfRecipes.push_back(recipe(in));
    }
    delete in;

    newRound();
	emit barOpened();
}

void BarModel::liquorPressed(QString liquorName) {
    pressedLiquor = true;
    liquorSelection = liquorName;
    bool found = false;
    QPair<QString, int> &ingredient = userRecipe.ingredients[stepNumber];
    if (ingredient.first == liquorSelection) {
        found = true;
    }
    if(!found){
        outOfOrder = true;
        userRecipe.ingredients.push_back(QPair<QString, int>(liquorSelection, -1));
    }

    // Start a single-shot timer with a delay of 1000 milliseconds (1 second)
    timer.start();
}

void BarModel::processLiquor() {
    if(pressedLiquor) { // Check if the whiskey button is still pressed
        for (QPair<QString, int> &ingredient : userRecipe.ingredients) {
            if (ingredient.first == liquorSelection) {
                ingredient.second--;
                qDebug() << liquorSelection << " " << ingredient.second;
                if(ingredient.second == 0) stepNumber++;
                qDebug() << "Step Number:" << stepNumber;
            }
        }
        if(!pressedLiquor) {
            return;
        }
    }
}

void BarModel::liquorReleased() {
    pressedLiquor = false;
    timer.stop();
}

void BarModel::ingredientClicked(QString ingredientName){

    QPair<QString, int> &ingredient = userRecipe.ingredients[stepNumber];
    if (ingredient.first == ingredientName) {
        ingredient.second--;
        qDebug() << ingredientName << " " << ingredient.second;
        if(ingredient.second == 0) stepNumber++;
        qDebug() << stepNumber;
    }
    else{
        userRecipe.ingredients.push_back(QPair<QString, int>(ingredientName, -1));
    }

}


void BarModel::newRound(){
    stepNumber = 0;
    outOfOrder = false;
    // duplicate recipes possible in this case currently.
    int randomNumber = QRandomGenerator::global()->bounded(listOfRecipes.size());
    while(true){
        if(assignedRecipe.drinkName != listOfRecipes[randomNumber].drinkName){
            break;
        }
        else{
            randomNumber = QRandomGenerator::global()->bounded(listOfRecipes.size());
        }
    }

    assignedRecipe = listOfRecipes[0]; // debug purposes
    userRecipe = assignedRecipe;
    //assignedRecipe = listOfRecipes[randomNumber];
    emit newDrink(assignedRecipe.recipeAsString);
}

void BarModel::serveDrink() {
    // check recipe validity
    bool correctDrink = userRecipe.checkServedDrink(assignedRecipe);
    // if it is correct call new round without resetting game
    if(correctDrink && !outOfOrder){
        newRound();
        qDebug() << "Congratulations!";
    }
    else{
        qDebug() << "Order is out of order: " << outOfOrder;
        qDebug() << "Correct drink: " << correctDrink;
    }
    // else maybe give a error that they made the drink incorrect and start new game or retry that recipe
}
