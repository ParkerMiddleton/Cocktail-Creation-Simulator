#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gamewindow.h"
#include <QtMultimedia>

// Forward declaration.
class BarModel;

QT_BEGIN_NAMESPACE
namespace Ui
{
/// @brief Educational Application main window.
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT
    // audio fields
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    QMediaPlayer *soundBoard;
    QAudioOutput *audioOutputSB;

public:
	/// @brief Constructor. Initializes the UI and all of the signal to slot connections.
    MainWindow(BarModel *bar, GameWindow *gw, QWidget *parent = nullptr);

	/// @brief Destructor.
	~MainWindow();

    void on_startGameButton_clicked();

public slots:
	/// @brief Prints the message to the console.
	void printMessage();
    void handleBackButtonClicked();

signals:
    void startGame();


private:
	Ui::MainWindow *ui;
	BarModel *bar;
    GameWindow *gameWindow;

};

#endif // MAINWINDOW_H
