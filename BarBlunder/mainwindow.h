#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// Forward declaration.
class BarModel;

QT_BEGIN_NAMESPACE
namespace Ui
{
/// @brief Educational Application main window.
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	/// @brief Constructor. Initializes the UI and all of the signal to slot connections.
	MainWindow(BarModel *bar, QWidget *parent = nullptr);

	/// @brief Destructor.
	~MainWindow();

public slots:
	/// @brief Prints the message to the console.
	void printMessage();

signals:


private:
	Ui::MainWindow *ui;
	BarModel *bar;

};

#endif // MAINWINDOW_H
