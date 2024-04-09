#include "mainwindow.h"
#include "barmodel.h"
#include <QApplication>

int main(int argc, char **argv)
{
	QApplication application(argc, argv);

	BarModel bar;
    GameWindow gameWindow;
    MainWindow window(&bar, &gameWindow);
	window.show();
	bar.initialize();


	return application.exec();
}
