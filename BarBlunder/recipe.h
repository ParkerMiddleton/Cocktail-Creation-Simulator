#ifndef RECIPE_H
#define RECIPE_H

#include <QString>
#include <QList>

class QTextStream;

/// @brief Holds technical and descriptive information about recipe. Part of Model.
class Recipe
{
public:
	/// @brief Name of this recipe's drink.
	QString drinkName;

	/// @brief Recipe steps.
	QList<QString> steps;

	/// @brief Recipe steps that require specific (time to perform)/(volume to pour).
	QList<QPair<QString, int>> ingredients;

	/// @brief Default constructor.
	Recipe() = default;

	/// @brief Constructor.
	/// Loads recipe information from the given text buffer.
	/// @param recipeText Text buffer.
	Recipe(QTextStream &recipeText);

	/// @brief Checks the given recipe of made drink against the correct recipe.
	/// @param correctRecipe Recipe with steps performed by the player.
	/// @return true if correct, false otherwise.
	bool checkServedDrink(const Recipe &correctRecipe);

};

#endif // RECIPE_H
