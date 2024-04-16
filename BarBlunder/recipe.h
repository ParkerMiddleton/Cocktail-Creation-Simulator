#ifndef RECIPE_H
#define RECIPE_H

#include <QString>
#include <QList>

class QTextStream;

class Recipe
{
public:
	Recipe();
	Recipe(QTextStream &recipeText);
	~Recipe();

	bool checkServedDrink(Recipe correctRecipe);

	QString drinkName;
	QList<QPair<QString, int>> ingredients;
	QString recipeAsString;
};

#endif // RECIPE_H
