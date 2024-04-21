#include "gamelayer.h"
#include "ui_gamelayer.h"

#include "applicationmodel.h"
#include "recipenote.h"

#include <QTimer>

//#include <QMediaPlayer>
//#include <QAudioOutput>

GameLayer::GameLayer(ApplicationModel *app, QWidget *parent)
    : QWidget{parent}
    , ui{new Ui::GameLayer}
{
	ui->setupUi(this);
	
	RecipeNote *recipeNote = ui->Recipe;
	recipeNote->setupLayout(ui->Steps);
	ui->v_Notes->setCurrentWidget(ui->Note1);

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
	ui->v_DrinkView->initializeConnections(bar);

    connect(ui->d_ServeDrinkButton, &QPushButton::clicked,
            bar, &::BarModel::serveDrink);

    connect(bar, &BarModel::newDrink
			, recipeNote, &RecipeNote::updateRecipe);

	connect(bar, &BarModel::correctIngredientUsed
			, recipeNote, &RecipeNote::showStepIsCorrect);

	connect(bar, &BarModel::incorrectIngredientUsed
			, recipeNote, &RecipeNote::showStepIsIncorrect);

	connect(bar, &BarModel::drinkIsCorrect
			, this, &GameLayer::showRoundEndCorrectMessage);

	connect(bar, &BarModel::drinkIsIncorrect
			, this, &GameLayer::showRoundEndIncorrectMessage);

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
    connect(ui->d_OrangeLiquorButton, &QPushButton::released,
            bar, &BarModel::ingredientReleased);

    connect(ui->d_KahluaButton, &QPushButton::pressed,
            bar, [bar](){bar->ingredientPressed("kahlua");
            });
    connect(ui->d_KahluaButton, &QPushButton::released,
            bar, &BarModel::ingredientReleased);

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

	connect(ui->d_WhiskeyButton, &QPushButton::pressed,
            bar, [bar]() {bar->ingredientPressed("whiskey");
            });
	connect(ui->d_WhiskeyButton, &QPushButton::released,
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

	connect(ui->d_BurbonButton, &QPushButton::pressed,
			bar, [bar]() {bar->ingredientPressed("burbon");
			});
	connect(ui->d_BurbonButton, &QPushButton::released,
			bar, &BarModel::ingredientReleased);

    //shakertin
    // should take away liquid while the user is holding down the shakertin
    connect(ui->d_ShakertinButton, &QPushButton::pressed,
            bar, &BarModel::hideDrink);

    connect(ui->d_ShakertinButton, &QPushButton::released,
            bar, &BarModel::exposeDrink);

}

GameLayer::~GameLayer()
{
    delete ui;
}

void GameLayer::showPauseOverlay()
{
    pauseOverlay->setVisible(true);
}

void GameLayer::hidePauseOverlay()
{
    pauseOverlay->setVisible(false);
}

void GameLayer::showRoundEndCorrectMessage()
{
	ui->RoundEndMessage->setText("Correct!");
	ui->v_Notes->setCurrentWidget(ui->Note2);
	QTimer::singleShot(ROUND_END_MESSAGE_DURATION_MS, this, &GameLayer::switchToRecipeNote);
}

void GameLayer::showRoundEndIncorrectMessage()
{
	ui->RoundEndMessage->setText("Incorrect!");
	ui->v_Notes->setCurrentWidget(ui->Note2);
	QTimer::singleShot(ROUND_END_MESSAGE_DURATION_MS, this, &GameLayer::switchToRecipeNote);
}

void GameLayer::switchToRecipeNote()
{
	ui->v_Notes->setCurrentWidget(ui->Note1);
}
