#include "mainwindow.h"
#include "applicationmodel.h"


#include <QApplication>

int main(int argc, char **argv)
{
	QApplication application(argc, argv);

	ApplicationModel app;
	MainWindow window(&app);
	window.show();
	app.initialize();



	return application.exec();
}
