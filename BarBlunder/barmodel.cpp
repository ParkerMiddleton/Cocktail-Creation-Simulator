#include "barmodel.h"
#include "recipe.h"
#include <random>
BarModel::BarModel(QObject *parent)
	: QObject{parent}
{

}

void BarModel::initialize()
{
    QFile file(":/drinks");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file from resources";
        return;
    }

    QTextStream* in = new QTextStream(&file);
    while (!in->atEnd()) {
        listOfRecipes.push_back(recipe(in));
    }
    delete in;

	emit barOpened();
}

void BarModel::whiskeyPressed(){

}

void BarModel::newRound(){
    int randomNumber = QRandomGenerator::global()->bounded(listOfRecipes.size());
    assignedRecipe = listOfRecipes(randomNumber);

}
