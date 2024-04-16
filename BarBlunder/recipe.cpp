#include "recipe.h"

#include <QTextStream>
#include <QRegularExpression>

Recipe::Recipe() {

}

Recipe::Recipe(QTextStream &recipe) {
	recipeAsString = "";
	QString line = recipe.readLine();
	while (!line.isEmpty()) {
		recipeAsString += line + "\n";
		line = recipe.readLine();
	}

	drinkName = recipe.readLine();
	line = recipe.readLine();
	while (!line.isEmpty()) {
		QRegularExpression re("(.+):\\s*(\\d+)");
		QRegularExpressionMatch match = re.match(line);
		if (match.hasMatch()) {
			QString ingredientName = match.captured(1);
			int quantity = match.captured(2).toInt();
			ingredients.push_back(qMakePair(ingredientName, quantity));
		}
		line = recipe.readLine();
	}
}

Recipe::~Recipe(){

}

bool Recipe::checkServedDrink(Recipe correctRecipe) {
	// Different number of ingredients, so it's not a match
	if (ingredients.size() != correctRecipe.ingredients.size()) {
		return false;
	}
	for (int i = 0; i < ingredients.size(); ++i) {
		// Check if the names of the ingredients match
		if (ingredients[i].first != correctRecipe.ingredients[i].first) {
			return false;
		}
		// Check if the served amount is within ±10 units of the correct amount
		if (ingredients[i].second != 0) {
			return false;
		}
	}
	return true;
}
