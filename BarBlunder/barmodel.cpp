#include "barmodel.h"
#include "recipe.h"

BarModel::BarModel(QObject *parent)
	: QObject{parent}
{
    timer.setInterval(1000); // Set interval to 1000 milliseconds (1 second)
    connect(&timer, &QTimer::timeout, this, &BarModel::processWhiskey);
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

void BarModel::whiskeyPressed() {
    pressedWhiskey = true;
    // Start a single-shot timer with a delay of 1000 milliseconds (1 second)
    timer.start();
}

void BarModel::processWhiskey() {
    if(pressedWhiskey) { // Check if the whiskey button is still pressed
        for (QPair<QString, int> &ingredient : userRecipe.ingredients) {
            if (ingredient.first == "whiskey" && ingredient.second > 0 && pressedWhiskey) {
                ingredient.second--;
                qDebug() << ingredient.second;
                // int volume ++
            }
        }
        if(!pressedWhiskey) {
            return;
        }
    }
}

void BarModel::whiskeyReleased() {
    pressedWhiskey = false;
    timer.stop();
}


void BarModel::rocksPressed() {

}

void BarModel::icePoured() {

}

void BarModel::bittersPressed() {

}

void BarModel::stirPressed() {

}

void BarModel::orangePeel() {

}

void BarModel::newRound(){
    // duplicate recipes possible in this case currently.
    int randomNumber = QRandomGenerator::global()->bounded(listOfRecipes.size());
    assignedRecipe = listOfRecipes[0]; // debug purposes
    userRecipe = assignedRecipe;
    //assignedRecipe = listOfRecipes[randomNumber];

}
