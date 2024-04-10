#ifndef MAINMENUPAGE_H
#define MAINMENUPAGE_H

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainMenuPage;
}
QT_END_NAMESPACE

class MainMenuPage : public QWidget
{
	Q_OBJECT

public:
	explicit MainMenuPage(QWidget *parent = nullptr);
	~MainMenuPage();

public slots:
	void onStartButtonClicked();

signals:
	void gameStartRequested();

private:
	Ui::MainMenuPage *ui;

};

#endif // MAINMENUPAGE_H
