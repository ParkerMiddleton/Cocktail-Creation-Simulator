#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "barmodel.h"

MainWindow::MainWindow(BarModel *bar, QWidget *parent)
	: QMainWindow{parent}
	, ui{new Ui::MainWindow}
	, bar{bar}
{
	ui->setupUi(this);

	// Filepath testing. Remove later.
	QPixmap sampleImagePixmap(":/images/sampleimage.png");
	qDebug() << sampleImagePixmap;

	// Connections
	connect(bar, &BarModel::barOpened
			, this, &MainWindow::printMessage);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::printMessage()
{
	QTextStream(stdout) << "Welcome!" << "\n";
}
