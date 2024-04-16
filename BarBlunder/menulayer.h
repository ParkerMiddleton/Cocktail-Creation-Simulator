#ifndef MENULAYER_H
#define MENULAYER_H

#include <QWidget>

// Forward declaration.
class MainMenuPage;
class SettingsMenuPage;
class ApplicationModel;

class QStackedWidget;
class QPropertyAnimation;

QT_BEGIN_NAMESPACE
namespace Ui
{
class MenuLayer;
}
QT_END_NAMESPACE

/// @brief Layer of menus above the game. Part of View.
class MenuLayer : public QWidget
{
	Q_OBJECT

public:
	/// @brief Constructor.
	/// Setups UI and connects all signals to slots.
	/// @param app Model responsible for managing states and settings.
	explicit MenuLayer(ApplicationModel *app, QWidget *parent = nullptr);

	/// @brief Destructor.
	~MenuLayer();

public slots:
	/// @brief Enabled Pause menu layout in MainMenuPage.
	void enablePauseMenuLayout();

	/// @brief Unhides the main overlay with main and settings menus with animation.
	void showMainOverlay();

	/// @brief Hides the main overlay with main and settings menus with animation.
	void hideMainOverlay();

	/// @brief Switches to MainMenuPage.
	void switchToMainMenu();

	/// @brief Switches to SettingsMenuPage.
	void switchToSettingsMenu();

private:
	Ui::MenuLayer *ui;

	QStackedWidget *menuStack;
	MainMenuPage *mainMenu;
	SettingsMenuPage *settingsMenu;

	QPropertyAnimation *triAnim;
	QPropertyAnimation *menuStackAnim;

	bool isPauseLayoutEnabled;

	/// @brief Helper method to initialize animation fields.
	void setupAnimations();

};

#endif // MENULAYER_H
