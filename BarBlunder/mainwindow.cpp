#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamewindow.h"
#include "barmodel.h"

MainWindow::MainWindow(BarModel *bar, GameWindow *gw, QWidget *parent)
    : QMainWindow{parent}
    , ui{new Ui::MainWindow}
    , bar{bar}
    , gameWindow(gw)
{
    ui->setupUi(this);

    // setup music
    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);
    player->setSource(QUrl("qrc:/sounds/lofi.mp3"));
    audioOutput->setVolume(10);
    player->setLoops(QMediaPlayer::Infinite);
    player->play();

    // Filepath testing. Remove later.
    //QPixmap sampleImagePixmap(":/images/sampleimage.png");
    //qDebug() << sampleImagePixmap;

    // Load the image into a QPixmap
    QPixmap backgroundImage(":/images/bar.png");

    // Scale the image to a smaller size
    QPixmap scaledImage = backgroundImage.scaled(800, 800, Qt::KeepAspectRatio);

    // Set the scaled image as the pixmap of the QLabel
    ui->menuBarImage->setPixmap(scaledImage);

    ui->menuBarImage->setAlignment(Qt::AlignCenter);

    // Connections
    connect(bar, &BarModel::barOpened
            , this, &MainWindow::printMessage);

    // Start Menu -> Game Window
    connect(ui->startButton, &QPushButton::clicked,
            this, &MainWindow::on_startGameButton_clicked);

    // Game Window -> Start Menu
    connect(gameWindow, &GameWindow::backButtonClicked,
            this, &MainWindow::handleBackButtonClicked);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::printMessage()
{
	QTextStream(stdout) << "Welcome!" << "\n";
}

void MainWindow::on_startGameButton_clicked()
{
    // stop start menu music
    player->stop();
    // start bar game music (some music we decide on for game music)
    player->setSource(QUrl("qrc:/sounds/ragtime.mp3"));
    player->play();
    // Hide start menu
    hide();
    // Show the GameWindow
    gameWindow->show();
}

void MainWindow::handleBackButtonClicked() {
    // stop bar game music
    player->stop();
    // start menu music
    player->setSource(QUrl("qrc:/sounds/lofi.mp3"));
    player->play();
    // Hide game window
    gameWindow->hide();
    // Show start menu
    this->show();
}
