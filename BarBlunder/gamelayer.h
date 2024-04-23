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

    void updatePourTimer(int time);

    void playSoundEffect(const QString &soundFilePath);
    void playPourSound(const QString &soundFilePath);

private slots:
	/// @brief Displays the recipe note.
	/// Used after the round end message was displayed for a specified amount of time.
	void switchToRecipeNote();

private:
	static constexpr int ROUND_END_MESSAGE_DURATION_MS = 1000;

	Ui::GameLayer *ui;

	QWidget *pauseOverlay;

	// Sound effects
    QMediaPlayer *soundBoard;
    QAudioOutput *audioOutputSB;
    void setupSoundBoard();


};

#endif // GAMELAYER_H
