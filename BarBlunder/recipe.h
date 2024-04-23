#ifndef RECIPE_H
#define RECIPE_H

#include <QString>
#include <QList>

class QTextStream;

class Recipe
{
public:
    /// @brief default constructior
	Recipe() = default;
    /**
     * @brief another constructor
     * @param recipeTextthe projected text for the recipe
     */
	Recipe(QTextStream &recipeText);
    /**
     * @brief checkServedDrink checks the served drink against the correct recipe
     * @param correctRecipe -  the correct recipe
     * @return is the served drink correct?
     */
	bool checkServedDrink(Recipe correctRecipe);
    /// @brief the name of the drink
	QString drinkName;
    /// @brief the steps of the recipes as a series of strings
	QList<QString> recipeSteps;
    /// @brief the ingredients in the recipe in amounts represented as a series of QPairs.
	QList<QPair<QString, int>> ingredients;

};

#endif // RECIPE_H
