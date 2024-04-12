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
    for (QPair<QString, int> &ingredient : userRecipe.ingredients) {
        if (ingredient.first == liquorSelection) {
            found = true;
        }
    }
    if(!found){
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
    bool found = false;
    for (QPair<QString, int> &ingredient : userRecipe.ingredients) {
        if (ingredient.first == ingredientName) {
            found = true;
            ingredient.second--;
            qDebug() << ingredientName << " " << ingredient.second;
        }
    }
    if(!found){
        userRecipe.ingredients.push_back(QPair<QString, int>(ingredientName, -1));
    }

}


void BarModel::newRound(){
    // duplicate recipes possible in this case currently.
    int randomNumber = QRandomGenerator::global()->bounded(listOfRecipes.size());
    assignedRecipe = listOfRecipes[0]; // debug purposes
    userRecipe = assignedRecipe;
    //assignedRecipe = listOfRecipes[randomNumber];

}
