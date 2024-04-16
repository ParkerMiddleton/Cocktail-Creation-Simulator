#include "gamelayer.h"
#include "ui_gamelayer.h"

#include "applicationmodel.h"

#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QMediaPlayer>
#include <QAudioOutput>

GameLayer::GameLayer(ApplicationModel *app, QWidget *parent)
	: QWidget{parent}
	, ui{new Ui::GameLayer}
{
	ui->setupUi(this);

	BarModel *bar = app->barModel();
	//this->setDisabled(true);

	connect(ui->PauseButton, &QPushButton::clicked,
			app, &ApplicationModel::pause);

	connect(app, &ApplicationModel::gamePaused,
			this, &GameLayer::showPauseOverlay);

	connect(app, &ApplicationModel::gameUnpaused,
			this, &GameLayer::hidePauseOverlay);

	/*===== SETUP PAUSE OVERLAY =====*/

	pauseOverlay = new QWidget(this);
	pauseOverlay->resize(this->size());
	pauseOverlay->raise();
	pauseOverlay->setStyleSheet("background-color: rgba(0, 0, 0, 76);");

	////////////// ICONS FOR BUTTONS /////////////////////

	//Lime Wedge Container
	QIcon limeIcon(":/images/lime-wedges-in-glass.png");
	ui->d_LimeWedgeButton->setIcon(limeIcon);
	ui->d_LimeWedgeButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	QSize limeButtonSize(ui->d_LimeWedgeButton->size());
	ui->d_LimeWedgeButton->setIconSize(limeButtonSize*1.3);

	// Orange Peele container
    QIcon orangeIcon(":/images/ButtonIcons/Garnish/orangePeeles.png");
	ui->d_OrangeButton->setIcon(orangeIcon);
	ui->d_OrangeButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	QSize orangeButtonSize(ui->d_OrangeButton->size());
	ui->d_OrangeButton->setIconSize(orangeButtonSize*1.7);

	// Olive container
	QIcon oliveIcon(":/images/olives.png");
	ui->d_OliveButton->setIcon(oliveIcon);
	ui->d_OliveButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	QSize oliveButtonSize(ui->d_OliveButton->size());
	ui->d_OliveButton->setIconSize(oliveButtonSize*1.7);

	//bitters bottle
    QIcon bittersIcon(":/images/ButtonIcons/Garnish/olives.png");
	ui->d_BittersButton->setIcon(bittersIcon);
	ui->d_BittersButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	QSize BittersButtonSize(ui->d_BittersButton->size());
	ui->d_BittersButton->setIconSize(BittersButtonSize*1.7);

	//shakertin
    QIcon shakertinIcon(":/images/ButtonIcons/Tools/shakertin.png");
	ui->d_ShakertinButton->setIcon(shakertinIcon);
	ui->d_ShakertinButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	QSize shakertinButtonSize(ui->d_ShakertinButton->size());
	ui->d_ShakertinButton->setIconSize(shakertinButtonSize*.95);

	// Connections
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


    //Liqour connections
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
}

GameLayer::~GameLayer()
{
	delete ui;
}

void GameLayer::showPauseOverlay()
{
	//this->setDisabled(true);
	pauseOverlay->setDisabled(false);
	pauseOverlay->setVisible(true);
}

void GameLayer::hidePauseOverlay()
{
	//this->setDisabled(false);
	pauseOverlay->setDisabled(true);
	pauseOverlay->setVisible(false);
}
void GameLayer::updateRecipebox(QString recipe)
{
	ui->d_DrinkAlgoBox->setText(recipe);
}
