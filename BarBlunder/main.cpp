//>>================== CS 3505 - Assignment 9: Educational App ==================<<
//
//>> Bar Blunder
//
//>>===========================================================================<<

#include "mainwindow.h"
#include "applicationmodel.h"

#include <QApplication>

int main(int argc, char **argv)
{
	QApplication QtApplication(argc, argv);

	ApplicationModel gameApplication;
	MainWindow window(&gameApplication);
	window.show();
	gameApplication.run();

	return QtApplication.exec();
}
