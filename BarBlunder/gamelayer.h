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

class GameLayer : public QWidget
{
	Q_OBJECT

public:
	explicit GameLayer(ApplicationModel *app, QWidget *parent = nullptr);
	~GameLayer();

public slots:
	void updateRecipebox(const QString &recipe);

	void showPauseOverlay();
	void hidePauseOverlay();

private:
	Ui::GameLayer *ui;

	QWidget *pauseOverlay;

	// Sound effects
	//QMediaPlayer *soundBoard;
	//QAudioOutput *audioOutputSB;

};

#endif // GAMELAYER_H
