#include "recipe.h"

recipe::recipe(QTextStream* recipe) {

    drinkName = recipe->readLine();
    QString line = recipe->readLine();
    QString builder;
    QPair<QString, int> ingredient;
    while(line != ""){
        bool num = false;
        foreach(QChar c, line){
            if(num){
                if(c != ' '){
                    builder += c;
                }
            }
            else if(c != ':') {
                builder += c;
            }
            else{
                ingredient.first = builder;
                builder = "";
                num = true;
            }
        }
        ingredient.second = builder.toInt();
        ingredients.push_back(ingredient);
        builder = "";
        line = recipe->readLine();
    }
}

recipe::~recipe(){

}
