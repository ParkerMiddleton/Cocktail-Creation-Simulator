#include "mainmenupage.h"
#include "ui_mainmenupage.h"

#include "applicationmodel.h"

MainMenuPage::MainMenuPage(QWidget *parent)
	: QWidget{parent}
	, ui{new Ui::MainMenu}
{
	ui->setupUi(this);
	ui->ResumeButton->setVisible(false);

	connect(ui->SettingsButton, &QPushButton::clicked,
			this, &MainMenuPage::onSettingsButtonClicked);
}

MainMenuPage::~MainMenuPage()
{
	delete ui;
}

void MainMenuPage::setupModelConnections(ApplicationModel *app)
{
	connect(ui->ResumeButton, &QPushButton::clicked,
			app, &ApplicationModel::unpause);

	connect(ui->BeginButton, &QPushButton::clicked,
			app, &ApplicationModel::startNewGame);

	connect(ui->QuitButton, &QPushButton::clicked,
			app, &ApplicationModel::exitApplication);
}

void MainMenuPage::showPauseMenuWidgets()
{
	ui->ResumeButton->setVisible(true);
	ui->BeginButton->setText("RESTART GAME");
}

void MainMenuPage::onSettingsButtonClicked()
{
	emit settingsButtonClicked();
}
