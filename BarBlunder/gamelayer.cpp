#include "gamelayer.h"
#include "ui_gamelayer.h"

#include "applicationmodel.h"

#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QMediaPlayer>
#include <QAudioOutput>

GamePage::GamePage(ApplicationModel *app, QWidget *parent)
	: QWidget{parent}
	, ui{new Ui::GameLayer}
{
	ui->setupUi(this);

	BarModel *bar = app->barModel();
	//this->setDisabled(true);

	connect(app, &ApplicationModel::gamePaused,
			this, &GamePage::showPauseOverlay);

	connect(app, &ApplicationModel::gameUnpaused,
			this, &GamePage::hidePauseOverlay);

	/*===== SETUP PAUSE OVERLAY =====*/

	pauseOverlay = new QWidget(this);
	pauseOverlay->resize(this->size());
	pauseOverlay->raise();
	pauseOverlay->setStyleSheet("background-color: rgba(0, 0, 0, 76);");

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
    ui->LimeWedgeButton->setIcon(limeIcon);
    ui->LimeWedgeButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    QSize limeButtonSize(ui->LimeWedgeButton->size());
    ui->LimeWedgeButton->setIconSize(limeButtonSize*1.3);

	// Orange Peele container
	QIcon orangeIcon(":/images/orangePeeles.png");
	ui->OrangeButton->setIcon(orangeIcon);
	ui->OrangeButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	QSize orangeButtonSize(ui->OrangeButton->size());
	ui->OrangeButton->setIconSize(orangeButtonSize*1.7);

	// Olive container
	QIcon oliveIcon(":/images/olives.png");
	ui->OliveButton->setIcon(oliveIcon);
	ui->OliveButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	QSize oliveButtonSize(ui->OliveButton->size());
	ui->OliveButton->setIconSize(oliveButtonSize*1.7);

	//bitters bottle
	QIcon bittersIcon(":/images/bitters.png");
	ui->BittersButton->setIcon(bittersIcon);
	ui->BittersButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	QSize BittersButtonSize(ui->BittersButton->size());
	ui->BittersButton->setIconSize(BittersButtonSize*1.7);

	//shakertin
	QIcon shakertinIcon(":/images/shakertin.png");
	ui->ShakertinButton->setIcon(shakertinIcon);
	ui->ShakertinButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	QSize shakertinButtonSize(ui->ShakertinButton->size());
	ui->ShakertinButton->setIconSize(shakertinButtonSize*.95);

	// ice cubes
	QIcon iceIcon(":/images/ice.png");
	ui->IceButton->setIcon(iceIcon);
	ui->IceButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	QSize iceButtonSize(ui->IceButton->size());
	ui->IceButton->setIconSize(iceButtonSize * 1.3);

	// Connections
    connect(ui->serveDrinkButton, &QPushButton::clicked,
            bar, &::BarModel::serveDrink);

    connect(bar, &BarModel::newDrink
            , this, &GamePage::updateRecipebox);

    // Button connections
    
    connect(ui->LimeWedgeButton, &QPushButton::clicked,
            bar, [bar]() {bar->ingredientClicked("lime wedge");
    });
    connect(ui->OrangeButton, &QPushButton::clicked,
            bar, [bar]() {bar->ingredientClicked("orange peeles");
    });
    connect(ui->OliveButton, &QPushButton::clicked,
            bar, [bar]() {bar->ingredientClicked("olives");
    });
    connect(ui->BittersButton, &QPushButton::clicked,
            bar, [bar]() {bar->ingredientClicked("bitters");
    });
    connect(ui->IceButton, &QPushButton::clicked,
            bar, [bar]() {bar->ingredientClicked("ice");
    });
    connect(ui->RocksGlassButton, &QPushButton::clicked,
            bar, [bar]() {bar->ingredientClicked("rocks glass");
    });
    connect(ui->CollinsGlassButton, &QPushButton::clicked,
            bar, [bar]() {bar->ingredientClicked("collins glass");
    });
    connect(ui->CopperMugButton, &QPushButton::clicked,
            bar, [bar]() {bar->ingredientClicked("copper mug");
    });
    connect(ui->MartiniGlassButton, &QPushButton::clicked,
            bar, [bar]() {bar->ingredientClicked("martini glass");
    });
    connect(ui->ShakertinButton, &QPushButton::clicked,
            bar, [bar]() {bar->ingredientClicked("shake");
    });
    connect(ui->StirButton, &QPushButton::clicked,
            bar, [bar]() {bar->ingredientClicked("stir");
    });
    connect(ui->SinkButton, &QPushButton::clicked,
            bar, &BarModel::emptyDrink);


    //Liqour connections
    connect(ui->OrangeLiquorButton, &QPushButton::pressed,
            bar, [bar](){ bar->ingredientPressed("orange liquor");
    });

    connect(ui->KahluaButton, &QPushButton::pressed,
            bar, [bar](){bar->ingredientPressed("kahlua");
    });

    connect(ui->VodkaButton, &QPushButton::pressed,
            bar, [bar]() {bar->ingredientPressed("vodka");
    });
    connect(ui->VodkaButton, &QPushButton::released,
            bar, &BarModel::ingredientReleased);

    connect(ui->TequilaButton, &QPushButton::pressed,
            bar, [bar]() {bar->ingredientPressed("tequila");
    });
    connect(ui->TequilaButton, &QPushButton::released,
            bar, &BarModel::ingredientReleased);

    connect(ui->GinButton, &QPushButton::pressed,
            bar, [bar]() {bar->ingredientPressed("gin");
    });
    connect(ui->GinButton, &QPushButton::released,
            bar, &BarModel::ingredientReleased);

    connect(ui->BurbonButton, &QPushButton::pressed,
            bar, [bar]() {bar->ingredientPressed("whiskey");
    });
    connect(ui->BurbonButton, &QPushButton::released,
            bar, &BarModel::ingredientReleased);

    connect(ui->RumButton, &QPushButton::pressed,
            bar, [bar]() {bar->ingredientPressed("rum");
    });
    connect(ui->RumButton, &QPushButton::released,
            bar, &BarModel::ingredientReleased);

    connect(ui->LimeJuiceButton, &QPushButton::pressed,
            bar, [bar]() {bar->ingredientPressed("lime juice");
    });
    connect(ui->LimeJuiceButton, &QPushButton::released,
            bar, &BarModel::ingredientReleased);

    connect(ui->GingerBeerButton, &QPushButton::pressed,
            bar, [ bar]() {bar->ingredientPressed("ginger beer");
    });
    connect(ui->GingerBeerButton, &QPushButton::released,
            bar, &BarModel::ingredientReleased);
}

GamePage::~GamePage()
{
	delete ui;
}

void GamePage::showPauseOverlay()
{
	//this->setDisabled(true);
	pauseOverlay->setVisible(true);
}

void GamePage::hidePauseOverlay()
{
	//this->setDisabled(false);
	pauseOverlay->setVisible(false);
}
void GamePage::updateRecipebox(QString recipe)
{
    ui->DrinkAlgoBox->setText(recipe);
}
