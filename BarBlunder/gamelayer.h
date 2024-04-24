#ifndef GAMELAYER_H
#define GAMELAYER_H

#include <QWidget>

// Forward Declaration.
class ApplicationModel;

class QMediaPlayer;
class QAudioOutput;

QT_BEGIN_NAMESPACE
namespace Ui
{
class GameLayer;
}
QT_END_NAMESPACE

/// @brief The viewport layer with game contents. Part of View.
/// @extends QWidget
class GameLayer : public QWidget
{
	Q_OBJECT

public:
	/// @brief Constructor.
	/// @param app Model responsible for managing states and settings.
	explicit GameLayer(ApplicationModel *app, QWidget *parent = nullptr);

	/// @brief Destructor.
	~GameLayer();

public slots:
	/// @brief Sets visible the transparent overlay that stops the game controls from receiving events.
	void showPauseOverlay();

	/// @brief Hides the transparent overlay that stops the game controls from receiving events.
	void hidePauseOverlay();

	/// @brief Displays a message about the correctly made drink inside the note.
	void showRoundEndCorrectMessage();

	/// @brief Displays a message about the incorrectly made drink inside the note.
	void showRoundEndIncorrectMessage();

	/// @brief Sets the displayed time value of the pouring timer.
	void updatePourTimer(int time);

	/// @brief Plays a single, short sound.
	/// @param soundFilePath Path and name of the sound to play.
	void playSoundEffect(const QString &soundFilePath);

	/// @brief Plays a continous sound.
	/// @param soundFilePath Path and name of the sound to play.
	void playPourSound(const QString &soundFilePath);

	/// @brief Makes the customer (aka drink guy) say a phrase if recipe step was done correctly.
	/// @param recipeStepNumber Number of the recipe step.
	void updateDrunkGuyTextCorrect(int recipeStepNumber);

	/// @brief Makes the customer (aka drink guy) say a phrase if recipe step was done incorrectly.
	void updateDrunkGuyTextIncorrect();

	/// @brief Makes the customer (aka drink guy) say a phrase when he orders a new drink.
	/// @param drinkName Ordered drink.
	void setDrinkGuyNewOrder(const QString &drinkName);

	/// @brief Initiates shaking animation.
	void shake();

	/// @brief Stops the shaking animation.
	void shakeRelease();

private slots:
	/// @brief Displays the recipe note.
	/// Used after the round end message was displayed for a specified amount of time.
	void switchToRecipeNote();

	/// @brief Advances shaking animation.
	/// @param originalX Original x.
	/// @param originalY Original y.
	void performShake(int orginalX, int originalY);

private slots:
	/// @brief If checked, shows ControlsLabel.s
	void checkControlsCheckBox();

private:
	static constexpr int ROUND_END_MESSAGE_DURATION_MS = 1000;
	static constexpr int FRANK_MESSAGE_DURATION_MS = 3000;

	Ui::GameLayer *ui;
	QWidget *pauseOverlay;
	QTimer *animationTimer;
	int currentRecipeStep;

	// Sound effects
	QMediaPlayer *soundBoardPour;
	QAudioOutput *audioOutputSBPour;
	QMediaPlayer *soundBoardClicked;
	QAudioOutput *audioOutputSBClicked;

	/// @brief Helper method to initialize audio fields.
	void setupSoundBoard();

};

#endif // GAMELAYER_H
