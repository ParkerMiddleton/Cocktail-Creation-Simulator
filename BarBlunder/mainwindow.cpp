#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mainmenupage.h"
#include "gamepage.h"

#include "barmodel.h"

MainWindow::MainWindow(BarModel *bar, QWidget *parent)
	: QMainWindow{parent}
	, currentState{State::Beginning}
	, ui{new Ui::MainWindow}
	, viewport{this}
	, viewportScene{0, 0, RENDER_NATIVE_WIDTH, RENDER_NATIVE_HEIGHT}
	, bar{bar}
{
	// Setup MainWindow UI.
	ui->setupUi(this);
	this->setWindowTitle("Bar Blunder");

	this->setupAudio();

	mainMenuPage = new MainMenuPage(this);
	gamePage = new GamePage(bar); // passing bar model to game page.
	this->setupViewport();

	// Connections
	connect(bar, &BarModel::barOpened
			, this, &MainWindow::printMessage);

	connect(mainMenuPage, &MainMenuPage::beginAnewRequested
			, this, &MainWindow::beginNewEdu);

	connect(mainMenuPage, &MainMenuPage::unpauseRequested
			, this, &MainWindow::unpause);

	connect(mainMenuPage, &MainMenuPage::quitRequested
			, this, &MainWindow::close);

	connect(gamePage, &GamePage::gameExitRequested
			, this, &MainWindow::pause);

	gamePage->setDisabled(true);
	this->playMainMenuMusic();
}

MainWindow::~MainWindow()
{
	if (player->isPlaying())
		player->stop();

	delete player;
	delete audioOutput;
	delete ui;
}

void MainWindow::printMessage()
{
	QTextStream(stdout) << "Welcome!" << "\n";
}

void MainWindow::beginNewEdu()
{
	currentState = State::Unpaused;

	// TODO: Setup Logic for setting up new instance of education.

	this->unpause(); // Temp
}

void MainWindow::pause()
{
	currentState = State::Paused;

	this->playMainMenuMusic();

	// Switch to main menu page.
	gamePage->setDisabled(true);
	mainMenuPage->show();
}

void MainWindow::unpause()
{
	currentState = State::Unpaused;

	this->playEduMusic();

	// Switch to game page.
	gamePage->setDisabled(false);
	mainMenuPage->hide();
}

void MainWindow::setupViewport()
{
	// Disable QGraphicsView scrollbars.
	viewport.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	viewport.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	viewport.horizontalScrollBar()->setEnabled(false);
	viewport.verticalScrollBar()->setEnabled(false);

	viewport.setStyleSheet("border: 0px;"\
						   "background-color: black;");
	this->setCentralWidget(&viewport);

	// Setup layers.
	viewportScene.addWidget(gamePage);
	viewportScene.addWidget(mainMenuPage);
	viewport.setScene(&viewportScene);
}

void MainWindow::setupAudio()
{
	player = new QMediaPlayer(this);
	audioOutput = new QAudioOutput(this);
	player->setAudioOutput(audioOutput);
	player->setLoops(QMediaPlayer::Infinite);
	audioOutput->setVolume(0);
}

void MainWindow::playMainMenuMusic()
{
	// Stop bar game music.
	if (player->isPlaying())
		player->stop();

	// Start main menu music.
	player->setSource(QUrl("qrc:/sounds/lofi.mp3"));
	player->play();
}

void MainWindow::playEduMusic()
{
	// Stop main menu music.
	if (player->isPlaying())
		player->stop();

	// Start bar game music (some music we decide on for game music).
	player->setSource(QUrl("qrc:/sounds/ragtime.mp3"));
	player->play();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
	const QSize &newSize = event->size();

	viewport.resetTransform();
	qreal scaleFactor = std::min((qreal)newSize.width() / (qreal)RENDER_NATIVE_WIDTH, (qreal)newSize.height() / (qreal)RENDER_NATIVE_HEIGHT);
	viewport.scale(scaleFactor, scaleFactor);

	QWidget::resizeEvent(event);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape)
	{
		if (currentState == State::Unpaused)
		{
			this->pause();
		}
		else if (currentState == State::Paused)
		{
			this->unpause();
		}
	}
}
