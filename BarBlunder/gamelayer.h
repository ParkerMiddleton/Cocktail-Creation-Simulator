#ifndef GAMELAYER_H
#define GAMELAYER_H

#include <QWidget>

// Forward Declaration.
class ApplicationModel;

class QPropertyAnimation;

QT_BEGIN_NAMESPACE
namespace Ui
{
class GameLayer;
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
    void updateRecipebox(QString recipe);

signals:

private:
	Ui::GameLayer *ui;

	QWidget *pauseOverlay;

};

#endif // GAMELAYER_H
