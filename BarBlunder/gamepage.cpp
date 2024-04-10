#include "gamepage.h"
#include "ui_gamepage.h"

GamePage::GamePage(QWidget *parent)
	: QWidget{parent}
	, ui{new Ui::GamePage}
{
	ui->setupUi(this);

	// Connections
	connect(ui->pushButton, &QPushButton::clicked,
			this, &GamePage::onBackButtonClicked);
}

GamePage::~GamePage()
{
	delete ui;
}

void GamePage::onBackButtonClicked()
{
	emit gameExitRequested();
}
