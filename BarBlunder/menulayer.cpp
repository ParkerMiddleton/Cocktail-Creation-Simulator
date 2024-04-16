#include "menulayer.h"
#include "ui_menulayer.h"

#include "mainmenupage.h"
#include "settingsmenupage.h"

#include "applicationmodel.h"

#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

MenuLayer::MenuLayer(ApplicationModel *app, QWidget *parent)
	: QWidget{parent}
	, ui{new Ui::MenuLayer}
	, isPauseLayoutEnabled{false}
{
	ui->setupUi(this);
	ui->triqa_pause->setVisible(false);
	ui->PauseButton->setVisible(false);

	menuStack = ui->MenuStack;
	mainMenu = ui->MainMenu;
	settingsMenu = ui->SettingsMenu;
	menuStack->setCurrentWidget(mainMenu);

	this->setupAnimations();

	// Connections.
	connect(ui->PauseButton, &QPushButton::clicked,
			app, &ApplicationModel::pause);

	connect(mainMenu, &MainMenuPage::settingsButtonClicked,
			this, &MenuLayer::switchToSettingsMenu);

	connect(settingsMenu, &SettingsMenuPage::backButtonClicked,
			this, &MenuLayer::switchToMainMenu);

	connect(app, &ApplicationModel::gameStarted,
			this, &MenuLayer::enablePauseMenuLayout);

	connect(app, &ApplicationModel::gamePaused,
			this, &MenuLayer::showMainOverlay);

	connect(app, &ApplicationModel::gameUnpaused,
			this, &MenuLayer::hideMainOverlay);

	mainMenu->setupModelConnections(app);
	settingsMenu->setupModelConnections(app);
}

MenuLayer::~MenuLayer()
{
	delete ui;
}

void MenuLayer::enablePauseMenuLayout()
{
	isPauseLayoutEnabled = true;
}

void MenuLayer::showMainOverlay()
{
	this->setVisible(true);

	// Disable game pause button.
	ui->PauseButton->setDisabled(true);

	if (isPauseLayoutEnabled)
	{
		mainMenu->showPauseMenuWidgets();
	}

	triAnim->setDirection(QAbstractAnimation::Backward);
	triAnim->setEasingCurve(QEasingCurve::InCirc);

	if (triAnim->state() == QAbstractAnimation::Stopped)
	{
		triAnim->start();
	}

	menuStack->setVisible(true);

	if (menuStackAnim->state() == QAbstractAnimation::Stopped)
	{
		menuStackAnim->start();
	}
}

void MenuLayer::hideMainOverlay()
{
	this->setVisible(false);

	// Enable game pause button.
	ui->PauseButton->setDisabled(false);

	triAnim->setDirection(QAbstractAnimation::Forward);
	triAnim->setEasingCurve(QEasingCurve::OutQuad);

	if (triAnim->state() == QAbstractAnimation::Stopped)
	{
		triAnim->start();
	}

	if (menuStackAnim->state() == QAbstractAnimation::Running)
	{
		menuStackAnim->stop();
		menuStack->move(0, 0);
	}

	if (menuStack->currentWidget() == settingsMenu)
	{
		menuStack->setCurrentWidget(mainMenu);
	}

	menuStack->setVisible(false);
}

void MenuLayer::switchToMainMenu()
{
	menuStack->setCurrentWidget(mainMenu);
}

void MenuLayer::switchToSettingsMenu()
{
	menuStack->setCurrentWidget(settingsMenu);
}

void MenuLayer::setupAnimations()
{
	triAnim = new QPropertyAnimation(ui->tri, "pos", this);
	triAnim->setDuration(350);
	triAnim->setStartValue(QPoint(0, 0));
	triAnim->setEndValue(QPoint(-808, 0)); // Prev: -768

	menuStackAnim = new QPropertyAnimation(menuStack, "pos", this);
	menuStackAnim->setDuration(250);
	menuStackAnim->setStartValue(QPoint(-50, 0));
	menuStackAnim->setEndValue(QPoint(0, 0));
	menuStackAnim->setEasingCurve(QEasingCurve::OutSine);
}
