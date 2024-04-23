#include "gamelayer.h"
#include "ui_gamelayer.h"

#include "applicationmodel.h"
#include "recipenote.h"

#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>

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

    //setup music
    this->setupSoundBoard();

	connect(ui->PauseButton, &QPushButton::clicked,
			app, &ApplicationModel::pause);

	connect(app, &ApplicationModel::newGameStarted,
			this, &GameLayer::hidePauseOverlay);

	connect(app, &ApplicationModel::gamePaused,
			this, &GameLayer::showPauseOverlay);

	connect(app, &ApplicationModel::gameUnpaused,
			this, &GameLayer::hidePauseOverlay);

    connect(app, &ApplicationModel::audioVolumeChanged,
            audioOutputSB, &QAudioOutput::setVolume);

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

    // update pouring timer
    connect(bar, &BarModel::elapsedTimePressed
            , this, &GameLayer::updatePourTimer);

// Glassware buttons connections.
// "buttonName" without "d_" prefix!
#define connectGlassware(buttonName) \
    connect(ui->d_##buttonName, &QPushButton::clicked, bar, &BarModel::updateGlassClicked);

    connectGlassware(CollinsGlassButton)
        connectGlassware(MartiniGlassButton)
        connectGlassware(RocksGlassButton)
        connectGlassware(CopperMugButton)

#undef connectGlassware


	// Ingredient buttons connections.
	// "buttonName" without "d_" prefix!
#define connectIngredient(buttonName, ingredientString)		\
	connect(ui->d_##buttonName, &QPushButton::clicked,		\
bar, [bar]() {bar->ingredientClicked(ingredientString);		\
});
	connectIngredient(LimeWedgeButton,		"lime wedge")
    connectIngredient(OrangeButton,			"orange peel")
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

	connectLiquor(KahluaButton,				"kahlua");
	connectLiquor(VodkaButton,				"vodka");
	connectLiquor(TequilaButton,			"tequila");
	connectLiquor(GinButton,				"gin");
	connectLiquor(WhiskeyButton,			"whiskey");
	connectLiquor(RumButton,				"rum");
	connectLiquor(LimeJuiceButton,			"lime juice");
	connectLiquor(GingerBeerButton,			"ginger beer");
	connectLiquor(BurbonButton,				"burbon");
    connectLiquor(HalfNHalf,				"half n half");
    connectLiquor(SweetNSourMix,            "sweet and sour mix");
    connectLiquor(Agave_Nectar,				"agave nectar");
    connectLiquor(ShakertinButton,          "shake");
    connectLiquor(Grenadine,                "grenadine");
    connectLiquor(SimpleSyrup,              "simple syrup");
    connectLiquor(Sprite,                   "sprite");
    connectLiquor(Coke,                     "coke");
    connectLiquor(OliveJuiceButton,               "olive juice");
    connectLiquor(StirButton,               "stir");
    connectLiquor(OrangeLiquorButton,       "orange liquor splash");


#undef connectLiquor

#define connectSoundButton(buttonName, soundFilePath) \
    connect(ui->d_##buttonName, &QPushButton::clicked, \
            this, [this]() { playSoundEffect(soundFilePath); })

    // TODO:: add more sounds for the different clicked ingriedients
    // TODO:: maybe add functionality for sound with ice emptying, and without
    connectSoundButton(IceButton,           "qrc:/soundBoard/ice_cubes.mp3");
    connectSoundButton(SinkButton,          "qrc:/soundBoard/empty_liquid.mp3");

#undef connectSoundButton

#define connectPourSoundButton(buttonName, soundFilePath) \
    connect(ui->d_##buttonName, &QPushButton::pressed, \
            this, [this]() { playPourSound(soundFilePath); }); \
        connect(ui->d_##buttonName, &QPushButton::released, \
                soundBoard, &QMediaPlayer::stop);

    // TODO:: add non ice drink mix sound, and add empty drink sound
    connectPourSoundButton(KahluaButton,        "qrc:/soundBoard/liquid_pour_2.mp3");
    connectPourSoundButton(VodkaButton,         "qrc:/soundBoard/liquid_pour_2.mp3");
    connectPourSoundButton(TequilaButton,       "qrc:/soundBoard/liquid_pour_2.mp3");
    connectPourSoundButton(GinButton,           "qrc:/soundBoard/liquid_pour_2.mp3");
    connectPourSoundButton(WhiskeyButton,       "qrc:/soundBoard/liquid_pour_2.mp3");
    connectPourSoundButton(RumButton,           "qrc:/soundBoard/liquid_pour_2.mp3");
    connectPourSoundButton(LimeJuiceButton,     "qrc:/soundBoard/liquid_pour_2.mp3");
    connectPourSoundButton(GingerBeerButton,    "qrc:/soundBoard/liquid_pour_2.mp3");
    connectPourSoundButton(BurbonButton,        "qrc:/soundBoard/liquid_pour_2.mp3");
    connectPourSoundButton(HalfNHalf,           "qrc:/soundBoard/liquid_pour_2.mp3");
    connectPourSoundButton(SweetNSourMix,       "qrc:/soundBoard/liquid_pour_2.mp3");
    connectPourSoundButton(Agave_Nectar,        "qrc:/soundBoard/liquid_pour_2.mp3");
    connectPourSoundButton(Grenadine,           "qrc:/soundBoard/liquid_pour_2.mp3");
    connectPourSoundButton(SimpleSyrup,         "qrc:/soundBoard/liquid_pour_2.mp3");
    connectPourSoundButton(OliveJuiceButton,          "qrc:/soundBoard/liquid_pour_2.mp3");
    connectPourSoundButton(OrangeLiquorButton,  "qrc:/soundBoard/liquid_pour_2.mp3");
    connectPourSoundButton(Sprite,              "qrc:/soundBoard/soda_pour.mp3");
    connectPourSoundButton(Coke,                "qrc:/soundBoard/soda_pour.mp3");
    connectPourSoundButton(ShakertinButton,     "qrc:/soundBoard/shaker_sound.mp3");
    connectPourSoundButton(StirButton,          "qrc:/soundBoard/ice_stir.mp3");


#undef connectPourSoundButton

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

void GameLayer::updatePourTimer(int time) {
    ui->PourTimer->setText("Time: " + QString::number(time));
}

void GameLayer::setupSoundBoard() {
    audioOutputSB = new QAudioOutput(this);
    soundBoard = new QMediaPlayer(this);
    soundBoard->setAudioOutput(audioOutputSB);
    audioOutputSB->setVolume(0.0f);
}

void GameLayer::playSoundEffect(const QString &soundFilePath) {
    soundBoard->stop();
    soundBoard->setSource(QUrl(soundFilePath));
    soundBoard->play();
}

void GameLayer::playPourSound(const QString &soundFilePath) {
    // allow for current sound to finish before restarting
    if (!soundBoard->isPlaying()) {
        soundBoard->setLoops(QMediaPlayer::Infinite);
        soundBoard->setSource(QUrl(soundFilePath));
        soundBoard->play();
    }
}
