#include "mainmenupage.h"
#include "ui_mainmenupage.h"

#include "applicationmodel.h"

MainMenuPage::MainMenuPage(ApplicationModel *app, QWidget *parent)
	: QWidget{parent}
	, ui{new Ui::MainMenu}
{
	ui->setupUi(this);
	ui->ResumeButton->setVisible(false);

	connect(ui->SettingsButton, &QPushButton::clicked,
			this, &MainMenuPage::onSettingsButtonClicked);

	// Model
	connect(ui->ResumeButton, &QPushButton::clicked,
			app, &ApplicationModel::unpause);

	connect(ui->BeginButton, &QPushButton::clicked,
			app, &ApplicationModel::startNewGame);

	connect(ui->QuitButton, &QPushButton::clicked,
			app, &ApplicationModel::exitApplication);
}

MainMenuPage::~MainMenuPage()
{
	delete ui;
}

void MainMenuPage::showPauseMenuWidgets()
{
	ui->ResumeButton->setVisible(true);
	ui->BeginButton->setText("RESTART");
}

void MainMenuPage::onSettingsButtonClicked()
{
	emit settingsButtonClicked();
}
