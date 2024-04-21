#ifndef GAMELAYER_H
#define GAMELAYER_H

#include <QWidget>

// Forward Declaration.
class ApplicationModel;

//class QMediaPlayer;
//class QAudioOutput;

QT_BEGIN_NAMESPACE
namespace Ui
{
class GameLayer;
}
QT_END_NAMESPACE

class GameLayer : public QWidget
{
	Q_OBJECT

public:
	explicit GameLayer(ApplicationModel *app, QWidget *parent = nullptr);
	~GameLayer();

public slots:
	void showPauseOverlay();
	void hidePauseOverlay();

	void showRoundEndCorrectMessage();
	void showRoundEndIncorrectMessage();

private slots:
	void switchToRecipeNote();

private:
	static constexpr int ROUND_END_MESSAGE_DURATION_MS = 1000;

	Ui::GameLayer *ui;

	QWidget *pauseOverlay;

	// Sound effects
	//QMediaPlayer *soundBoard;
	//QAudioOutput *audioOutputSB;

};

#endif // GAMELAYER_H
