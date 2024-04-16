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
    QString recipeAsString;
    bool checkServedDrink(recipe correctRecipe);
};

#endif // RECIPE_H
