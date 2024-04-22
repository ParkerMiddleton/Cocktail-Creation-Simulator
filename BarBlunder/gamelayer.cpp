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

	BarModel *bar = app->barModel();
	ui->v_DrinkView->initializeConnections(bar);

	// Serve and Empty Connections.
	connect(ui->d_ServeDrinkButton, &QPushButton::clicked,
			bar, &::BarModel::serveDrink);

	connect(ui->d_SinkButton, &QPushButton::clicked,
			bar, &BarModel::emptyDrink);

	// Recipe Note Connections.
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

	// Ingredient buttons connections.
	// "buttonName" without "d_" prefix!
#define connectIngredient(buttonName, ingredientString)		\
	connect(ui->d_##buttonName, &QPushButton::clicked,		\
bar, [bar]() {bar->ingredientClicked(ingredientString);		\
});
	connectIngredient(LimeWedgeButton,		"lime wedge")
    connectIngredient(OrangeButton,			"orange peele")
	connectIngredient(OliveButton,			"olives")
	connectIngredient(BittersButton,		"bitters")
	connectIngredient(IceButton,			"ice")
	connectIngredient(RocksGlassButton,		"rocks glass")
	connectIngredient(CollinsGlassButton,	"collins glass")
	connectIngredient(CopperMugButton,		"copper mug")
	connectIngredient(MartiniGlassButton,	"martini glass")
    connectIngredient(OrangeLiquorButton,   "orange liquor splash")
	connectIngredient(StirButton,			"stir")

#undef connectIngredient

	// Liquor/Liquids buttons connections.
	// "buttonName" without "d_" prefix!
#define connectLiquor(buttonName, liquorString)						\
	connect(ui->d_##buttonName, &QPushButton::pressed,				\
			bar, [bar](){ bar->ingredientPressed(liquorString);		\
			});														\
	connect(ui->d_##buttonName, &QPushButton::released,				\
			bar, &BarModel::ingredientReleased);

	connectLiquor(OrangeLiquorButton,		"orange liquor");
	connectLiquor(KahluaButton,				"kahlua");
	connectLiquor(VodkaButton,				"vodka");
	connectLiquor(TequilaButton,			"tequila");
	connectLiquor(GinButton,				"gin");
	connectLiquor(WhiskeyButton,			"whiskey");
	connectLiquor(RumButton,				"rum");
	connectLiquor(LimeJuiceButton,			"lime juice");
	connectLiquor(GingerBeerButton,			"ginger beer");
	connectLiquor(BurbonButton,				"burbon");
<<<<<<< Updated upstream
    connectLiquor(HalfNHalf,				"half n half");
    connectLiquor(SweetNSourMix,            "sweet and sour mix");
    connectLiquor(Agave_Nectar,				"agave nectar");
    connectLiquor(ShakertinButton,          "shake");
=======
    connectLiquor(Grenadine,                "grenadine");
    connectLiquor(Agave_Nectar,             "agave nectar")
    connectLiquor(SimpleSyrup,              "simple syrup");
    connectLiquor(Sprite,                   "sprite");
    connectLiquor(Coke,                     "coke");
    connectLiquor(SweetNSourMix,            "sweet n sour");
    connectLiquor(HalfNHalf,                "half n half");
>>>>>>> Stashed changes


#undef connectLiquor
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
	ui->RoundEndMessage->setText("Incorrect.");
	ui->v_Notes->setCurrentWidget(ui->Note2);
	QTimer::singleShot(ROUND_END_MESSAGE_DURATION_MS, this, &GameLayer::switchToRecipeNote);
}

void GameLayer::switchToRecipeNote()
{
	ui->v_Notes->setCurrentWidget(ui->Note1);
}
