#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gamelayer.h"
#include "applicationmodel.h"

#include <QSize>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QScrollBar>
#include <QPropertyAnimation>
#include <QMediaPlayer>
#include <QAudioOutput>

MainWindow::MainWindow(ApplicationModel *app, QWidget *parent)
	: QMainWindow{parent}
	, ui{new Ui::MainWindow}
	, viewport{this}
	, viewportScene{0, 0, RENDER_NATIVE_WIDTH, RENDER_NATIVE_HEIGHT}
	, windowSize{RENDER_NATIVE_WIDTH, RENDER_NATIVE_HEIGHT}
	, gameLayer{app}
	, mainMenu{app}
	, settingsMenu{app}
	, isOverlayMenuPauseLayoutEnabled{false}
{
	// Setup MainWindow UI.
	ui->setupUi(this);
	this->setWindowTitle("Bar Blunder");
	this->setFixedSize(windowSize);

	// Setup viewport.
	viewport.horizontalScrollBar()->setEnabled(false);
	viewport.verticalScrollBar()->setEnabled(false);
	viewport.setStyleSheet("border: 0px; background-color: black;");
	this->setCentralWidget(&viewport);

	// Setup overlay menu.
	tri.resize({OVERLAY_MENU_WIDTH, OVERLAY_MENU_HEIGHT});
	tri.setStyleSheet("background-color: transparent; image: url(:/images/menu/tri);");
	tri.setFocusPolicy(Qt::NoFocus);

	menuStack.resize({OVERLAY_MENU_WIDTH, OVERLAY_MENU_HEIGHT});
	menuStack.setStyleSheet("background-color: transparent;");
	menuStack.addWidget(&mainMenu);
	menuStack.addWidget(&settingsMenu);

	// Setup layers.
	viewportScene.addWidget(&gameLayer);
	viewportScene.addWidget(&tri);
	viewportScene.addWidget(&menuStack);
	viewport.setScene(&viewportScene);

	// Setup Animations
	this->setupOverlayMenuAnimations();

	// Setup audio.
	this->setupMusic();

	// Connections for the overlay menu.
	connect(&mainMenu, &MainMenuPage::settingsButtonClicked,
			this, &MainWindow::switchOverlayMenuToSettings);

	connect(&settingsMenu, &SettingsMenuPage::backButtonClicked,
			this, &MainWindow::switchOverlayMenuToMain);

	connect(app, &ApplicationModel::newGameStarted,
			this, &MainWindow::enableOverlayMenuPauseLayout);

	connect(app, &ApplicationModel::gamePaused,
			this, &MainWindow::showOverlayMenu);

	connect(app, &ApplicationModel::gameUnpaused,
			this, &MainWindow::hideOverlayMenu);

	// Connections for the window itself.
	connect(app, &ApplicationModel::newGameStarted,
			this, &MainWindow::setupNewGame);

	connect(app, &ApplicationModel::gamePaused,
			this, &MainWindow::playMenuMusic);

	connect(app, &ApplicationModel::gameUnpaused,
			this, &MainWindow::playGameMusic);

	connect(app, &ApplicationModel::audioVolumeChanged,
			menuMusicOutput, &QAudioOutput::setVolume);

	connect(app, &ApplicationModel::audioVolumeChanged,
			gameMusicOutput, &QAudioOutput::setVolume);

	connect(app, &ApplicationModel::audioVolumeChanged,
			gameVolumeFadeInAnim, &QPropertyAnimation::setEndValue);

	connect(app, &ApplicationModel::fullscreenModeChanged,
			this, &MainWindow::setFullscreenMode);

	connect(app, &ApplicationModel::windowSizeChanged,
			this, &MainWindow::setSize);

	connect(app, &ApplicationModel::applicationExitRequested,
			this, &MainWindow::close);

	connect(this, &MainWindow::escapeKeyPressed,
			app, &ApplicationModel::switchPauseState);

	// Play music.
	this->playMenuMusic();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::enableOverlayMenuPauseLayout()
{
	isOverlayMenuPauseLayoutEnabled = true;
}

void MainWindow::showOverlayMenu()
{
	if (isOverlayMenuPauseLayoutEnabled)
	{
		mainMenu.showPauseMenuWidgets();
	}

	triAnim->setDirection(QAbstractAnimation::Backward);
	triAnim->setEasingCurve(QEasingCurve::InCirc);

	if (triAnim->state() == QAbstractAnimation::Stopped)
	{
		triAnim->start();
	}

	menuStack.setVisible(true);

	if (menuStackAnim->state() == QAbstractAnimation::Stopped)
	{
		menuStackAnim->start();
	}
}

void MainWindow::hideOverlayMenu()
{
	triAnim->setDirection(QAbstractAnimation::Forward);
	triAnim->setEasingCurve(QEasingCurve::OutQuad);

	if (triAnim->state() == QAbstractAnimation::Stopped)
	{
		triAnim->start();
	}

	if (menuStackAnim->state() == QAbstractAnimation::Running)
	{
		menuStackAnim->stop();
		menuStack.move(0, 0);
	}

	if (menuStack.currentWidget() == &settingsMenu)
	{
		menuStack.setCurrentWidget(&mainMenu);
	}

	menuStack.setVisible(false);
}

void MainWindow::switchOverlayMenuToMain()
{
	menuStack.setCurrentWidget(&mainMenu);
}

void MainWindow::switchOverlayMenuToSettings()
{
	menuStack.setCurrentWidget(&settingsMenu);
}

void MainWindow::setupNewGame()
{
	menuMusic->pause();

	if (gameMusic->isPlaying())
	{
		gameVolumeFadeInAnim->stop();
		gameMusic->stop();
	}

	gameMusic->setSource(QUrl("qrc:/music/ragtime.mp3"));
	gameMusic->play();
	this->hideOverlayMenu();
}

void MainWindow::playMenuMusic()
{
	// Pause game music.
	if (gameMusic->isPlaying())
	{
		gameVolumeFadeInAnim->stop();
		gameMusic->pause();
	}

	// Start menu music.
	menuMusic->setPosition(0);
	menuMusic->play();
}

void MainWindow::playGameMusic()
{
	// Pause menu music.
	if (menuMusic->isPlaying())
		menuMusic->pause();

	// Resume game music.
	gameVolumeFadeInAnim->start();
	gameMusic->play();
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

void MainWindow::setupOverlayMenuAnimations()
{
	triAnim = new QPropertyAnimation(&tri, "pos", this);
	triAnim->setDuration(350);
	triAnim->setStartValue(QPoint(0, 0));
	triAnim->setEndValue(QPoint(-808, 0)); // Prev: -768

	menuStackAnim = new QPropertyAnimation(&menuStack, "pos", this);
	menuStackAnim->setDuration(250);
	menuStackAnim->setStartValue(QPoint(-50, 0));
	menuStackAnim->setEndValue(QPoint(0, 0));
	menuStackAnim->setEasingCurve(QEasingCurve::OutSine);
}

void MainWindow::setupMusic()
{
	menuMusicOutput = new QAudioOutput(this);
	menuMusic = new QMediaPlayer(this);
	menuMusic->setAudioOutput(menuMusicOutput);
	menuMusic->setLoops(QMediaPlayer::Infinite);
	menuMusicOutput->setVolume(0.0f);
	menuMusic->setSource(QUrl("qrc:/music/lofi.mp3"));

	gameMusicOutput = new QAudioOutput(this);
	gameMusic = new QMediaPlayer(this);
	gameMusic->setAudioOutput(gameMusicOutput);
	gameMusic->setLoops(QMediaPlayer::Infinite);
	gameMusicOutput->setVolume(0.0f);

	gameVolumeFadeInAnim = new QPropertyAnimation(gameMusicOutput, "volume", this);
	gameVolumeFadeInAnim->setDuration(350);
	gameVolumeFadeInAnim->setStartValue(0.0f);
	gameVolumeFadeInAnim->setEndValue(gameMusicOutput->volume());
}
