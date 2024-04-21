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
/**
 * @class the layer we play the game on
 * @package Ui
 */
class GameLayer;
}
QT_END_NAMESPACE

/**
 * @brief The GameLayer class reperesnts the layer we play the game on
 */
class GameLayer : public QWidget
{
	Q_OBJECT

public:
    /**
     * @brief GameLayer - a constructor
     * @param app - an app model
     * @param parent -- a widget parent defaults to null
     */
	explicit GameLayer(ApplicationModel *app, QWidget *parent = nullptr);
    /// @brief destructor for GameLayer
	~GameLayer();

public slots:
    /**
     * @brief updateRecipebox updates the recipe display
     * @param recipe - this is a ref to what recipe you use
     */
	void updateRecipebox(const QString &recipe);
    /// @brief shows the pause overlay to pause game
	void showPauseOverlay();
    /// @brief hides the pause overlay to resume game
	void hidePauseOverlay();

private:
    /// @brief a pointer to the ui
	Ui::GameLayer *ui;

    /// @brief this is a pointer to a QWidget which pauses the overlay
	QWidget *pauseOverlay;

	// Sound effects
	//QMediaPlayer *soundBoard;
	//QAudioOutput *audioOutputSB;

};

#endif // GAMELAYER_H
