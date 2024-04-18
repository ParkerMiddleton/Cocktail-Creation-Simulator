#ifndef GAMELAYER_H
#define GAMELAYER_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "Box2D/Box2D.h"
#include "Glassware.h"
#include <QTimer>
// Forward Declaration.
class ApplicationModel;

class QMediaPlayer;
class QAudioOutput;

QT_BEGIN_NAMESPACE
namespace Ui
{
class GameLayer;
}
QT_END_NAMESPACE

class GameLayer : public QWidget
{
	Q_OBJECT

public:
	explicit GameLayer(ApplicationModel *app, QWidget *parent = nullptr);
	~GameLayer();

public slots:
	void updateRecipebox(const QString &recipe);

	void showPauseOverlay();
	void hidePauseOverlay();

    //updates physics numnbers per iteration
    void updateWorld();

    //spawn glassware
    void spawnRocksGlass();
    void spawnCollinsGlass();
    void spawnMartiniGlass();


signals:
    void heightChanged(int x , int y);

private:
	Ui::GameLayer *ui;

	QWidget *pauseOverlay;

    QGraphicsScene* scene;
    Glassware* glass;

    //surface physics
    b2BodyDef* surfaceBodyDef;
    b2Body* surfaceBody;
    b2FixtureDef* surfaceFixtureDef;
    b2PolygonShape* surfaceBox;

    //glass physics
    b2BodyDef* glassBodyDef;
    b2Body* glassBody;
    b2FixtureDef* glassFixtureDef;
    b2PolygonShape* glassShape;


    b2World* world;
    //update world Timer
    QTimer timer;





	// Sound effects
	//QMediaPlayer *soundBoard;
	//QAudioOutput *audioOutputSB;

};

#endif // GAMELAYER_H
