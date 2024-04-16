#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "menulayer.h"
#include "gamepage.h"

#include "applicationmodel.h"

#include <QSize>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QScrollBar>
#include <QMediaPlayer>
#include <QAudioOutput>

MainWindow::MainWindow(ApplicationModel *app, QWidget *parent)
	: QMainWindow{parent}
	, ui{new Ui::MainWindow}
	, viewport{this}
	, viewportScene{0, 0, RENDER_NATIVE_WIDTH, RENDER_NATIVE_HEIGHT}
	, windowSize{RENDER_NATIVE_WIDTH, RENDER_NATIVE_HEIGHT}
{
	// Setup MainWindow UI.
	ui->setupUi(this);
	this->setWindowTitle("Bar Blunder");
	this->setFixedSize(windowSize);

	// Setup viewport.
	viewport.horizontalScrollBar()->setEnabled(false);
	viewport.verticalScrollBar()->setEnabled(false);
	viewport.setStyleSheet("border: 0px; background-color: black;");
	//viewport.setFocusPolicy(Qt::NoFocus);
	this->setCentralWidget(&viewport);

	// Setup layers.
	gameLayer = new GamePage(app);
	menuLayer = new MenuLayer(app);
	viewportScene.addWidget(gameLayer);
	viewportScene.addWidget(menuLayer);
	viewport.setScene(&viewportScene);

	// Setup audio.
	this->setupAudio();

	// Connections
	connect(app, &ApplicationModel::gamePaused
			, this, &MainWindow::playMenuMusic);

	connect(app, &ApplicationModel::gameUnpaused
			, this, &MainWindow::playGameMusic);

	connect(app, &ApplicationModel::audioVolumeChanged
			, audioOutput, &QAudioOutput::setVolume);

	connect(app, &ApplicationModel::fullscreenModeChanged
			, this, &MainWindow::setFullscreenMode);

	connect(app, &ApplicationModel::windowSizeChanged
			, this, &MainWindow::setSize);

	connect(app, &ApplicationModel::applicationExitRequested
			, this, &MainWindow::close);

	connect(this, &MainWindow::escapeKeyPressed
			, app, &ApplicationModel::switchPauseState);

	// Play music.
	this->playMenuMusic();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::playMenuMusic()
{
	// Stop bar game music.
	if (player->isPlaying())
		player->stop();

	// Start main menu music.
	player->setSource(QUrl("qrc:/sounds/lofi.mp3"));
	player->play();
}

void MainWindow::playGameMusic()
{
	// Stop main menu music.
	if (player->isPlaying())
		player->stop();

	// Start bar game music (some music we decide on for game music).
	player->setSource(QUrl("qrc:/sounds/ragtime.mp3"));
	player->play();
}

void MainWindow::setFullscreenMode(bool state)
{
	if (state && !this->isFullScreen())
	{
		QSize screenSize = QApplication::primaryScreen()->size();
		this->setFixedSize(screenSize);
		this->showFullScreen();
	}
	else if (!state && this->isFullScreen())
	{
		this->showNormal();
		this->setFixedSize(windowSize);
	}
}

void MainWindow::setSize(const QSize &newSize)
{
	QPoint positionCenter;
	positionCenter.setX(this->pos().x() + (this->width() / 2));
	positionCenter.setY(this->pos().y() + (this->height() / 2));

	windowSize = newSize;
	this->setFixedSize(newSize);

	// Adjust position.
	QPoint newPosition;
	newPosition.setX(positionCenter.x() - (this->width() / 2));
	newPosition.setY(positionCenter.y() - (this->height() / 2));
	this->move(newPosition);
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
		emit escapeKeyPressed();
	}
}

void MainWindow::setupAudio()
{
	player = new QMediaPlayer(this);
	audioOutput = new QAudioOutput(this);
	player->setAudioOutput(audioOutput);
	player->setLoops(QMediaPlayer::Infinite);
	audioOutput->setVolume(0.0f);
}
