#include "gamewindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameWindow)
{
    ui->setupUi(this);


    ////////////// ICONS FOR BUTTONS /////////////////////

    //Vodka
    QIcon vodkaIcon(":/images/titos.png");
    ui->VodkaButton->setIcon(vodkaIcon);
    ui->VodkaButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    QSize vodkaButtonSize(ui->VodkaButton->size());
    ui->VodkaButton->setIconSize(vodkaButtonSize * 2.85);

    //Tequila
    QIcon tequilaIcon(":/images/tequila.png");
    ui->TequilaButton->setIcon(tequilaIcon);
    ui->TequilaButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    QSize tequilaButtonSize(ui->TequilaButton->size());
    ui->TequilaButton->setIconSize(tequilaButtonSize * 2.8);

    //Gin
    QIcon ginIcon(":/images/gin.png");
    ui->GinButton->setIcon(ginIcon);
    ui->GinButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    QSize ginButtonSize(ui->GinButton->size());
    ui->GinButton->setIconSize(ginButtonSize * .85);

    //Burbon
    QIcon burbonIcon(":/images/burbon.png");
    ui->BurbonButton->setIcon(burbonIcon);
    ui->BurbonButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    QSize burbonButtonSize(ui->BurbonButton->size());
    ui->BurbonButton->setIconSize(burbonButtonSize * 2.3);

    //Rum
    QIcon rumIcon(":/images/rum.png");
    ui->RumButton->setIcon(rumIcon);
    ui->RumButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    QSize rumButtonSize(ui->RumButton->size());
    ui->RumButton->setIconSize(rumButtonSize * 2.5);

    //Lime Wedge Container
    QIcon limeIcon(":/images/lime-wedges-in-glass.png");
    ui->LimeButton->setIcon(limeIcon);
    ui->LimeButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    QSize limeButtonSize(ui->LimeButton->size());
    ui->LimeButton->setIconSize(limeButtonSize*1.7);


}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::on_pushButton_clicked()
{
    emit backButtonClicked();
}

