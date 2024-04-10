#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mainmenupage.h"
#include "gamepage.h"

#include "barmodel.h"

MainWindow::MainWindow(BarModel *bar, QWidget *parent)
	: QMainWindow{parent}
	, ui{new Ui::MainWindow}
	, bar{bar}
{
	ui->setupUi(this);

	// Setup pages.
	pageStack = new QStackedWidget();

	mainMenuPage = new MainMenuPage(this);
	gamePage = new GamePage(this);

	pageStack->addWidget(mainMenuPage);
	pageStack->addWidget(gamePage);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(pageStack);
	this->setLayout(layout);
	this->setCentralWidget(pageStack);

	// Setup music and its volume.
	player = new QMediaPlayer;
	audioOutput = new QAudioOutput;
	player->setAudioOutput(audioOutput);
	player->setLoops(QMediaPlayer::Infinite);
	audioOutput->setVolume(0);

	// Filepath testing. Remove later.
	//QPixmap sampleImagePixmap(":/images/sampleimage.png");
	//qDebug() << sampleImagePixmap;

	// Connections
	connect(bar, &BarModel::barOpened
			, this, &MainWindow::printMessage);

	connect(mainMenuPage, &MainMenuPage::gameStartRequested
			, this, &MainWindow::switchToGamePage);

	connect(gamePage, &GamePage::gameExitRequested
			, this, &MainWindow::switchToMainMenuPage);

	this->switchToGamePage();
}

MainWindow::~MainWindow()
{
	player->stop();
	delete player;
	delete audioOutput;
	delete ui;
}

void MainWindow::printMessage()
{
	QTextStream(stdout) << "Welcome!" << "\n";
}

void MainWindow::switchToMainMenuPage()
{
	// Stop bar game music.
	if (player->isPlaying())
		player->stop();

	// Start main menu music.
	player->setSource(QUrl("qrc:/sounds/lofi.mp3"));
	player->play();

	// Switch to main menu page.
	this->pageStack->setCurrentWidget(mainMenuPage);
}

void MainWindow::switchToGamePage()
{
	// Stop main menu music.
	if (player->isPlaying())
		player->stop();

	// Start bar game music (some music we decide on for game music).
	player->setSource(QUrl("qrc:/sounds/ragtime.mp3"));
	player->play();

	// Switch to game page.
	this->pageStack->setCurrentWidget(gamePage);
}
