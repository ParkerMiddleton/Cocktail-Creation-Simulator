#ifndef GAMEPAGE_H
#define GAMEPAGE_H
#include "barmodel.h"
QT_BEGIN_NAMESPACE
namespace Ui
{
class GamePage;
}
QT_END_NAMESPACE

class GamePage : public QWidget
{
	Q_OBJECT
public:
    explicit GamePage(QWidget *parent = nullptr, BarModel *bar);
	~GamePage();

public slots:
	void onBackButtonClicked();

signals:
	void gameExitRequested();

private:
    BarModel model;
	Ui::GamePage *ui;

};

#endif // GAMEPAGE_H
