#include "recipe.h"

#include <QTextStream>
#include <QRegularExpression>

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
