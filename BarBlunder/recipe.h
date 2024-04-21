#ifndef RECIPE_H
#define RECIPE_H

#include <QString>
#include <QList>

class QTextStream;

class Recipe
{
public:
	Recipe() = default;
	Recipe(QTextStream &recipeText);

	bool checkServedDrink(Recipe correctRecipe);

	QString drinkName;
	QList<QString> recipeSteps;
	QList<QPair<QString, int>> ingredients;

};

#endif // RECIPE_H
