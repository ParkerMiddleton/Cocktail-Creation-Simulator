#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Forward declaration.
class MainMenuPage;
class GamePage;
class BarModel;

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

/// @brief Educational Application main window.
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	/// @brief Constructor. Initializes the UI and all of the signal to slot connections.
	MainWindow(BarModel *bar, QWidget *parent = nullptr);

	/// @brief Destructor.
	~MainWindow();

public slots:
	/// @brief Prints the message to the console.
	void printMessage();

	void switchToMainMenuPage();
	void switchToGamePage();

private:
	Ui::MainWindow *ui;

	QStackedWidget *pageStack;
	MainMenuPage *mainMenuPage;
	GamePage *gamePage;

	BarModel *bar;

	// Audio Fields
	QMediaPlayer *player;
	QAudioOutput *audioOutput;

	// For sound effects.
	//QMediaPlayer *soundBoard;
	//QAudioOutput *audioOutputSB;

};

#endif // MAINWINDOW_H
