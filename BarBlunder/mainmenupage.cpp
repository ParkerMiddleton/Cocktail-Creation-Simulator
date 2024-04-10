#include "mainmenupage.h"
#include "ui_mainmenupage.h"

MainMenuPage::MainMenuPage(QWidget *parent)
	: QWidget{parent}
	, ui{new Ui::MainMenuPage}
{
	ui->setupUi(this);

	QPixmap backgroundImage(":/images/bar.png");
	QPixmap scaledImage = backgroundImage.scaled(800, 800, Qt::KeepAspectRatio);
	ui->menuBarImage->setPixmap(scaledImage);
	ui->menuBarImage->setAlignment(Qt::AlignCenter);

	// Connections
	connect(ui->startButton, &QPushButton::clicked,
			this, &MainMenuPage::onStartButtonClicked);
}

MainMenuPage::~MainMenuPage()
{
	delete ui;
}

void MainMenuPage::onStartButtonClicked()
{
	emit gameStartRequested();
}
