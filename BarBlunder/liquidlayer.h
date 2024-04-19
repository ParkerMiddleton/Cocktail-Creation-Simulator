#ifndef LIQUIDLAYER_H
#define LIQUIDLAYER_H

#include <QWidget>
#include <QTimer>
#include <Box2D/Box2D.h>
#include "liquidmodel.h"
#include "ui_liquidlayer.h"

class ApplicationModel;

QT_BEGIN_NAMESPACE
namespace Ui
{
class LiquidLayer;
}
class LiquidLayer : public QWidget
{
    Q_OBJECT

public:
    LiquidLayer(ApplicationModel *app, QWidget *parent = nullptr);
    ~LiquidLayer();

private slots:
    void updateSimulation();
    void setVolume(int v);
    void setDrinkColor(QString drinkName);
    void emptyDrink();

signals:
    void emptyLiquid();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::LiquidLayer *ui; // Now Ui::LiquidPoor should be fully defined
    void setupBox2D();
    void setupLiquidFun();
    LiquidModel* l;
    int volume;
    QString currentDrink;
    QMap<QString, QColor> drinkColors;
    bool isDrinkEmpty;
    b2World* m_world;
    QTimer* m_timer;
    QPixmap pixmap;
    void displayPauseButton();
    void hidePauseButton();
};

#endif // LIQUIDPOUR_H
