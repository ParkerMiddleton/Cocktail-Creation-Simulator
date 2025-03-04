#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gameLayer.h"
#include "mainmenupage.h"
#include "settingsmenupage.h"

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>

#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>

#include <QList>
#include <QMediaPlayer>

// Forward declaration.
class ApplicationModel;

class QPropertyAnimation;
class QAudioOutput;

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

/// @brief Educational Application main window. Part of View.
/// @extends QMainWindow
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	/// @brief Constructor.
	/// Initializes the UI and connections.
	/// @param app Model responsible for managing states and settings.
	MainWindow(ApplicationModel *app, QWidget *parent = nullptr);

	/// @brief Destructor.
	~MainWindow();

public slots:
	/// @brief Enabled Pause menu layout in MainMenuPage.
	void enableOverlayMenuPauseLayout();

	/// @brief Unhides the overlay menu with main and settings menus with animation.
	void showOverlayMenu();

	/// @brief Hides the overlay menu with main and settings menus with animation.
	void hideOverlayMenu();

	/// @brief Switches the overlay menu to MainMenuPage.
	void switchOverlayMenuToMain();

	/// @brief Switches the overlay menu to SettingsMenuPage.
	void switchOverlayMenuToSettings();

	/// @brief Resets the game music and hides the overlay menu.
	void setupNewGame();

	/// @brief Stops game music and starts playing menu music.
	void playMenuMusic();

	/// @brief Stops menu music and starts playing game music.
	void playGameMusic();

	/// @brief Enables and disables fullscreen mode.
	/// @param state If true, enables. If false, disables.
	void setFullscreenMode(bool state);

	/// @brief Sets a new size for window mode.
	/// @param newSize New size.
	void setSize(const QSize &newSize);

protected:
	void resizeEvent(QResizeEvent *event) override;

private slots:
	/// @brief Called when current game music end.
	/// Picks a new one.
	/// @param status Player status.
	void checkGamePlayerStatus(QMediaPlayer::MediaStatus status);

private:
	static constexpr int RENDER_NATIVE_WIDTH = 1280;
	static constexpr int RENDER_NATIVE_HEIGHT = 720;

	static constexpr int OVERLAY_MENU_WIDTH = 768;
	static constexpr int OVERLAY_MENU_HEIGHT = 720;

	Ui::MainWindow *ui;

	QGraphicsView viewport;
	QGraphicsScene viewportScene;
	QSize windowSize;

	GameLayer gameLayer;
	QLabel tri;
	QStackedWidget menuStack;
	MainMenuPage mainMenu;
	SettingsMenuPage settingsMenu;
	bool isOverlayMenuPauseLayoutEnabled;

	// Overlay Menu Animation
	QPropertyAnimation *triAnim;
	QPropertyAnimation *menuStackAnim;

	// Music
	QAudioOutput *menuMusicOutput;
	QAudioOutput *gameMusicOutput;
	QMediaPlayer *menuMusic;
	QMediaPlayer *gameMusic;
	QPropertyAnimation *gameVolumeFadeInAnim;
	QList<QString> gameMusicList;

	/// @brief Helper method to initialize animation fields.
	void setupOverlayMenuAnimations();

	/// @brief Helper method to initialize audio fields.
	void setupMusic();

	/// @brief Randomly selects game music.
	void pickNewGameMusic();

};

#endif // MAINWINDOW_H
