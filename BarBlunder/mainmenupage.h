#ifndef MAINMENUPAGE_H
#define MAINMENUPAGE_H

#include <QWidget>

// Forward Declaration.
class MenuLayer;
class ApplicationModel;

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainMenu;
}
QT_END_NAMESPACE

/// @brief Main(as well as Pause) Menu. Part of View.
class MainMenuPage : public QWidget
{
	Q_OBJECT

public:
	/// @brief Constructor.
	/// Setups UI and connects private slots to signals.
	explicit MainMenuPage(QWidget *parent = nullptr);

	/// @brief Destructor.
	~MainMenuPage();

	/// @brief Connects signals to ApplicationModel slots.
	void setupModelConnections(ApplicationModel *app);

	/// @brief Transforms Main menu into Pause menu.
	void showPauseMenuWidgets();

signals:
	/// @brief Notifies that SettingsButton was clicked.
	void settingsButtonClicked();

private slots:
	/// @brief Sends a signal if SettingsButton was clicked.
	void onSettingsButtonClicked();

private:
	Ui::MainMenu *ui;

};

#endif // MAINMENUPAGE_H
