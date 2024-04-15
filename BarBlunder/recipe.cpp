#include "recipe.h"

recipe::recipe() {

}

recipe::recipe(QTextStream* recipe) {
    drinkName = recipe->readLine();
    QString line = recipe->readLine();
    while (!line.isEmpty()) {
        QRegularExpression re("(.+):\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(line);
        qDebug() << match.captured(1) << " " << match.captured(2);
        if (match.hasMatch()) {
            QString ingredientName = match.captured(1);
            int quantity = match.captured(2).toInt();
            ingredients.push_back(qMakePair(ingredientName, quantity));
        }
        line = recipe->readLine();
    }
}

recipe::~recipe(){

}
void recipe::addIngredient(QString ingredient, int amount){
    ingredients.push_back(qMakePair(ingredient, amount));//add a pair to ingredients
}
bool recipe::checkRecipe(recipe drinkRecipe){
    /*
     * make sure both lists of ingredients are the same size
     */
    if (ingredients.size() != drinkRecipe.ingredients.size()) {
        return false;
    }
    /*
     * Check each Qpair in the drink is identical to the recipe
     */
    for (int i = 0; i < ingredients.size(); ++i) {
        if ((ingredients[i].first != drinkRecipe.ingredients[i].first)||(ingredients[i].second != drinkRecipe.ingredients[i].second)){
            return false;
        }
    }
    return true;//if we got this far in the method return true
}
