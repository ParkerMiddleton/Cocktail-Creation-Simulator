#include "mainwindow.h"
#include "barmodel.h"

int main(int argc, char **argv)
{
	QApplication application(argc, argv);

	BarModel bar;
	MainWindow window(&bar);
	window.show();
	bar.initialize();

	return application.exec();
}
