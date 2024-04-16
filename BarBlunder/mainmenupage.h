#ifndef MAINMENUPAGE_H
#define MAINMENUPAGE_H

// Forward declaration.
class MenuButton;
class MainWindow;

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainMenuPage;
}
QT_END_NAMESPACE

class MainMenuPage : public QWidget
{
	Q_OBJECT

public:
	explicit MainMenuPage(QWidget *parent);
	~MainMenuPage();

	void show();
	void hide();

public slots:
	// Buttons
	void onResumeButtonClicked();
	void onBeginButtonClicked();

	// Connections

private slots:
	void onSettingsButtonClicked();
	void onQuitButtonClicked();
	void onBackButtonClicked();

signals:
	void unpauseRequested();
    void beginAnewRequested();
	void quitRequested();

private:
	Ui::MainMenuPage *ui;

	QPropertyAnimation *triAnim;
	QPropertyAnimation *fadeOutAnim;

	QStackedWidget *pages;
	QWidget *menuPage;
	QWidget *settingsPage;

	// Menu
	QLabel *titleLabel;
	MenuButton *resumeButton;
	MenuButton *beginButton;
	MenuButton *settingsButton;
	MenuButton *quitButton;

	// Settings
	QSpinBox *volumeSpinBox;
	QComboBox *windowSizeComboBox;
	MenuButton *backButton;

	void setupMenuPage();
	void setupSettingsPage();
	void setupAnimations();

};

#endif // MAINMENUPAGE_H
