#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QWidget>

// Forward Declaration.
class ApplicationModel;

class QPropertyAnimation;

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
	explicit GamePage(ApplicationModel *app, QWidget *parent = nullptr);
	~GamePage();

public slots:
	void showPauseOverlay();
	void hidePauseOverlay();

signals:

private:
	Ui::GamePage *ui;

	QWidget *pauseOverlay;

};

#endif // GAMEPAGE_H
