#include "gamelayer.h"
#include "ui_gamelayer.h"

#include "applicationmodel.h"

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QGraphicsScene>
#include <QGraphicsItem>

GameLayer::GameLayer(ApplicationModel *app, QWidget *parent)
    : QWidget{parent}
    , ui{new Ui::GameLayer}
{
    ui->setupUi(this);

    ui->d_RocksGlassButtonSpawn->setVisible(false);


    // Pause overlay and button.
    pauseOverlay = new QWidget(this);
    pauseOverlay->resize(this->size());
    pauseOverlay->raise();
    pauseOverlay->setStyleSheet("background-color: rgba(0, 0, 0, 76);");

    connect(ui->PauseButton, &QPushButton::clicked,
            app, &ApplicationModel::pause);

    connect(app, &ApplicationModel::newGameStarted,
            this, &GameLayer::hidePauseOverlay);

    connect(app, &ApplicationModel::gamePaused,
            this, &GameLayer::showPauseOverlay);

    connect(app, &ApplicationModel::gameUnpaused,
            this, &GameLayer::hidePauseOverlay);

    // Connections
    BarModel *bar = app->barModel();

    connect(ui->d_ServeDrinkButton, &QPushButton::clicked,
            bar, &::BarModel::serveDrink);

    connect(bar, &BarModel::newDrink
            , this, &GameLayer::updateRecipebox);

    // Button connections
    connect(ui->d_LimeWedgeButton, &QPushButton::clicked,
            bar, [bar]() {bar->ingredientClicked("lime wedge");
            });
    connect(ui->d_OrangeButton, &QPushButton::clicked,
            bar, [bar]() {bar->ingredientClicked("orange peeles");
            });
    connect(ui->d_OliveButton, &QPushButton::clicked,
            bar, [bar]() {bar->ingredientClicked("olives");
            });
    connect(ui->d_BittersButton, &QPushButton::clicked,
            bar, [bar]() {bar->ingredientClicked("bitters");
            });
    connect(ui->d_IceButton, &QPushButton::clicked,
            bar, [bar]() {bar->ingredientClicked("ice");
            });
    connect(ui->d_RocksGlassButton, &QPushButton::clicked,
            bar, [bar]() {bar->ingredientClicked("rocks glass");
            });
    connect(ui->d_CollinsGlassButton, &QPushButton::clicked,
            bar, [bar]() {bar->ingredientClicked("collins glass");
            });
    connect(ui->d_CopperMugButton, &QPushButton::clicked,
            bar, [bar]() {bar->ingredientClicked("copper mug");
            });
    connect(ui->d_MartiniGlassButton, &QPushButton::clicked,
            bar, [bar]() {bar->ingredientClicked("martini glass");
            });
    connect(ui->d_ShakertinButton, &QPushButton::clicked,
            bar, [bar]() {bar->ingredientClicked("shake");
            });
    connect(ui->d_StirButton, &QPushButton::clicked,
            bar, [bar]() {bar->ingredientClicked("stir");
            });
    connect(ui->d_SinkButton, &QPushButton::clicked,
            bar, &BarModel::emptyDrink);

    // Liqour connections
    connect(ui->d_OrangeLiquorButton, &QPushButton::pressed,
            bar, [bar](){ bar->ingredientPressed("orange liquor");
            });

    connect(ui->d_KahluaButton, &QPushButton::pressed,
            bar, [bar](){bar->ingredientPressed("kahlua");
            });

    connect(ui->d_VodkaButton, &QPushButton::pressed,
            bar, [bar]() {bar->ingredientPressed("vodka");
            });
    connect(ui->d_VodkaButton, &QPushButton::released,
            bar, &BarModel::ingredientReleased);

    connect(ui->d_TequilaButton, &QPushButton::pressed,
            bar, [bar]() {bar->ingredientPressed("tequila");
            });
    connect(ui->d_TequilaButton, &QPushButton::released,
            bar, &BarModel::ingredientReleased);

    connect(ui->d_GinButton, &QPushButton::pressed,
            bar, [bar]() {bar->ingredientPressed("gin");
            });
    connect(ui->d_GinButton, &QPushButton::released,
            bar, &BarModel::ingredientReleased);

    connect(ui->d_BurbonButton, &QPushButton::pressed,
            bar, [bar]() {bar->ingredientPressed("whiskey");
            });
    connect(ui->d_BurbonButton, &QPushButton::released,
            bar, &BarModel::ingredientReleased);

    connect(ui->d_RumButton, &QPushButton::pressed,
            bar, [bar]() {bar->ingredientPressed("rum");
            });
    connect(ui->d_RumButton, &QPushButton::released,
            bar, &BarModel::ingredientReleased);

    connect(ui->d_LimeJuiceButton, &QPushButton::pressed,
            bar, [bar]() {bar->ingredientPressed("lime juice");
            });
    connect(ui->d_LimeJuiceButton, &QPushButton::released,
            bar, &BarModel::ingredientReleased);

    connect(ui->d_GingerBeerButton, &QPushButton::pressed,
            bar, [ bar]() {bar->ingredientPressed("ginger beer");
            });
    connect(ui->d_GingerBeerButton, &QPushButton::released,
            bar, &BarModel::ingredientReleased);

    // spawn glass
    connect(ui->d_RocksGlassButton, &QPushButton::clicked,
            this, &GameLayer::spawnRocksGlass);

    connect(ui->d_CopperMugButton, &QPushButton::clicked,
            this, &GameLayer::spawnRocksGlass);

    // despawn glass
    // connect(bar, &BarModel::informEmptyDrink,
    //         this, &GameLayer::emptyRocksGlass);


    //updates world view
    /*onnect(&timer, &QTimer::timeout, this, &GameLayer::updateWorld);
    timer.start(1000 / 60);*/ // 60 updates per second.

    // connect(this, &GameLayer::heightChanged
    //         , glass, &QGraphicsItem::setPos);


}

void GameLayer::setLiquid() {


}




GameLayer::~GameLayer()
{
    delete ui;
    // delete surfaceBodyDef;
    // delete surfaceBox;

}

void GameLayer::spawnRocksGlass() {
    ui->d_RocksGlassButtonSpawn->setVisible(true);
}

void GameLayer::emptyRocksGlass() {
    ui->d_RocksGlassButtonSpawn->setVisible(false);
}

void GameLayer::updateRecipebox(const QString &recipe)
{
    ui->d_DrinkAlgoBox->setText(recipe);
}

void GameLayer::showPauseOverlay()
{
    pauseOverlay->setVisible(true);
}

void GameLayer::hidePauseOverlay()
{
    pauseOverlay->setVisible(false);
}

//////////////// PARKERS CODE BELOW, UNSURE IF WE NEED THIS ATM LEAVING IT JUST INCASE /////////////////////////////////////////////
///
// void GameLayer::spawnRocksGlass(){
//     //rocks glass specs
//     QPolygonF rocks;
//     rocks
//         <<QPointF(50,50)
//         <<QPointF(60,50)
//         <<QPointF(60,90)
//         <<QPointF(90,90)
//         <<QPointF(90,50)
//         <<QPointF(100,50)
//         <<QPointF(100,100)
//         <<QPointF(50,100);

//     //removes the old glass, if adctive

//     if(glass){
//         scene->removeItem(glass);
//         delete glass;
//         glass = nullptr;
//     }

//     //assign to current glass
//     glass= new Glassware(rocks);
//     scene->addItem(glass);
//     scene->update();

// }


// void GameLayer::spawnCollinsGlass(){
//     //rocks glass specs
//     QPolygonF collins;
//     collins
//         <<QPointF(50,0)
//         <<QPointF(60,0)
//         <<QPointF(60,90)
//         <<QPointF(90,90)
//         <<QPointF(90,0)
//         <<QPointF(100,0)
//         <<QPointF(100,100)
//         <<QPointF(50,100);

//     //removes the old glass, if adctive

//     if(glass){
//         scene->removeItem(glass);
//         delete glass;
//         glass = nullptr;
//     }

//     //assign to current glass
//     glass= new Glassware(collins);
//     scene->addItem(glass);
//     scene->update();

// }


// void GameLayer::spawnMartiniGlass(){
//     //rocks glass specs
//     QPolygonF martini;
//     martini
//         <<QPointF(50,50)
//         <<QPointF(50,40)
//         <<QPointF(74,80)
//         <<QPointF(76,80)
//         <<QPointF(100,40)
//         <<QPointF(100,50)
//         <<QPointF(76,90)
//         <<QPointF(76,130)
//         <<QPointF(100,130)
//         <<QPointF(100,131)
//         <<QPointF(50,131)
//         <<QPointF(50,130)
//         <<QPointF(74,130)
//         <<QPointF(74,90);


//     //removes the old glass, if adctive

//     if(glass){
//         scene->removeItem(glass);
//         delete glass;
//         glass = nullptr;
//     }

//     //assign to current glass
//     glass= new Glassware(martini);
//     scene->addItem(glass);
//     scene->update();

// }


