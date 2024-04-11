#ifndef RECIPE_H
#define RECIPE_H

class recipe
{
public:

    recipe(QTextStream* recipeText);
    ~recipe();
    QString drinkName;
    QList<QPair<QString, int>> ingredients;
};

#endif // RECIPE_H
