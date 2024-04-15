#ifndef RECIPE_H
#define RECIPE_H

class recipe
{
public:
    recipe();
    recipe(QTextStream* recipeText);
    ~recipe();
    QString drinkName;
    QList<QPair<QString, int>> ingredients;
    /**
     * @brief This adds a specifird amount of the ingridient to the drink object
     * @param ingredient-the ingredient represented as a QString
     * @param amount-the amount of the ingredient
     */
    void addIngredient(QString ingredient, int amount);
    /**
     * @brief This method checks the users drink against the recipe
     * @param drinkRecipe - the actual correct recipe ffor the drink
     * @return true if the drink matches the recipe falsse if it does not
     */
    bool checkRecipe(recipe drinkRecipe);

};

#endif // RECIPE_H
