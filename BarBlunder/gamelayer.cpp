#include "gamelayer.h"
#include "ui_gamelayer.h"

#include "applicationmodel.h"

#include <QMediaPlayer>
#include <QAudioOutput>

GameLayer::GameLayer(ApplicationModel *app, QWidget *parent)
	: QWidget{parent}
	, ui{new Ui::GameLayer}
{
	ui->setupUi(this);

	// Pause overlay and button.
	pauseOverlay = new QWidget(this);
	pauseOverlay->resize(this->size());
	pauseOverlay->raise();
	pauseOverlay->setStyleSheet("background-color: rgba(0, 0, 0, 76);");

	connect(ui->PauseButton, &QPushButton::clicked,
			app, &ApplicationModel::pause);

	connect(app, &ApplicationModel::newGameStarted,
			this, &GameLayer::hidePauseOverlay);

	connect(app, &ApplicationModel::gamePaused,
			this, &GameLayer::showPauseOverlay);

	connect(app, &ApplicationModel::gameUnpaused,
			this, &GameLayer::hidePauseOverlay);

	// Connections
	BarModel *bar = app->barModel();

	connect(ui->d_ServeDrinkButton, &QPushButton::clicked,
			bar, &::BarModel::serveDrink);

	connect(bar, &BarModel::newDrink
			, this, &GameLayer::updateRecipebox);

	// Button connections
	connect(ui->d_LimeWedgeButton, &QPushButton::clicked,
			bar, [bar]() {bar->ingredientClicked("lime wedge");
			});
	connect(ui->d_OrangeButton, &QPushButton::clicked,
			bar, [bar]() {bar->ingredientClicked("orange peeles");
			});
	connect(ui->d_OliveButton, &QPushButton::clicked,
			bar, [bar]() {bar->ingredientClicked("olives");
			});
	connect(ui->d_BittersButton, &QPushButton::clicked,
			bar, [bar]() {bar->ingredientClicked("bitters");
			});
	connect(ui->d_IceButton, &QPushButton::clicked,
			bar, [bar]() {bar->ingredientClicked("ice");
			});
	connect(ui->d_RocksGlassButton, &QPushButton::clicked,
			bar, [bar]() {bar->ingredientClicked("rocks glass");
			});
	connect(ui->d_CollinsGlassButton, &QPushButton::clicked,
			bar, [bar]() {bar->ingredientClicked("collins glass");
			});
	connect(ui->d_CopperMugButton, &QPushButton::clicked,
			bar, [bar]() {bar->ingredientClicked("copper mug");
			});
	connect(ui->d_MartiniGlassButton, &QPushButton::clicked,
			bar, [bar]() {bar->ingredientClicked("martini glass");
			});
	connect(ui->d_ShakertinButton, &QPushButton::clicked,
			bar, [bar]() {bar->ingredientClicked("shake");
			});
	connect(ui->d_StirButton, &QPushButton::clicked,
			bar, [bar]() {bar->ingredientClicked("stir");
			});
	connect(ui->d_SinkButton, &QPushButton::clicked,
			bar, &BarModel::emptyDrink);

	// Liqour connections
	connect(ui->d_OrangeLiquorButton, &QPushButton::pressed,
			bar, [bar](){ bar->ingredientPressed("orange liquor");
			});

	connect(ui->d_KahluaButton, &QPushButton::pressed,
			bar, [bar](){bar->ingredientPressed("kahlua");
			});

	connect(ui->d_VodkaButton, &QPushButton::pressed,
			bar, [bar]() {bar->ingredientPressed("vodka");
			});
	connect(ui->d_VodkaButton, &QPushButton::released,
			bar, &BarModel::ingredientReleased);

	connect(ui->d_TequilaButton, &QPushButton::pressed,
			bar, [bar]() {bar->ingredientPressed("tequila");
			});
	connect(ui->d_TequilaButton, &QPushButton::released,
			bar, &BarModel::ingredientReleased);

	connect(ui->d_GinButton, &QPushButton::pressed,
			bar, [bar]() {bar->ingredientPressed("gin");
			});
	connect(ui->d_GinButton, &QPushButton::released,
			bar, &BarModel::ingredientReleased);

	connect(ui->d_BurbonButton, &QPushButton::pressed,
			bar, [bar]() {bar->ingredientPressed("whiskey");
			});
	connect(ui->d_BurbonButton, &QPushButton::released,
			bar, &BarModel::ingredientReleased);

	connect(ui->d_RumButton, &QPushButton::pressed,
			bar, [bar]() {bar->ingredientPressed("rum");
			});
	connect(ui->d_RumButton, &QPushButton::released,
			bar, &BarModel::ingredientReleased);

	connect(ui->d_LimeJuiceButton, &QPushButton::pressed,
			bar, [bar]() {bar->ingredientPressed("lime juice");
			});
	connect(ui->d_LimeJuiceButton, &QPushButton::released,
			bar, &BarModel::ingredientReleased);

	connect(ui->d_GingerBeerButton, &QPushButton::pressed,
			bar, [ bar]() {bar->ingredientPressed("ginger beer");
			});
	connect(ui->d_GingerBeerButton, &QPushButton::released,
			bar, &BarModel::ingredientReleased);





    /***********************PARKER MESS AROUND SPACE ***************/


    QPolygonF rocks;
    rocks
        <<QPointF(50,50)
        <<QPointF(60,50)
        <<QPointF(60,90)
        <<QPointF(90,90)
        <<QPointF(90,50)
        <<QPointF(100,50)
        <<QPointF(100,100)
        <<QPointF(50,100);

    QPolygonF collins;
    collins
        <<QPointF(50,0)
        <<QPointF(60,0)
        <<QPointF(60,90)
        <<QPointF(90,90)
        <<QPointF(90,0)
        <<QPointF(100,0)
        <<QPointF(100,100)
        <<QPointF(50,100);

    QPolygonF martini;
    martini
        <<QPointF(50,50)
        <<QPointF(50,40)
        <<QPointF(74,80)
        <<QPointF(76,80)
        <<QPointF(100,40)
        <<QPointF(100,50)
        <<QPointF(76,90)
        <<QPointF(76,130)
        <<QPointF(100,130)
        <<QPointF(100,131)
        <<QPointF(50,131)
        <<QPointF(50,130)
        <<QPointF(74,130)
        <<QPointF(74,90);

    // instantiate the scene
    scene  = new QGraphicsScene(this);

    // //Create the glassware object
    glass = new Glassware(collins);

    //add the glassware to the gview
    scene->addItem(glass);
    ui->drinkGraphicsView->setScene(scene);
    ui->drinkGraphicsView->show();


    //create the "world"
    //values here are gravity.
    world = new b2World(b2Vec2(0.0, -9.8f));

    //create a surface first..
    //what attributes should this body have? - more related to physical qualities
    surfaceBodyDef = new b2BodyDef();
    surfaceBodyDef->position.Set(0.0, -10.0f);
    surfaceBodyDef->type = b2_staticBody;
    //add it to the world
    surfaceBody = world->CreateBody(surfaceBodyDef); // this instantiates the body

    // Define the ground box shape.
    surfaceBox = new b2PolygonShape();
    // The extents are the half-widths of the box.
    surfaceBox->SetAsBox(500.0f, 10.0f);

    //define the fixture to the surface
    surfaceFixtureDef = new b2FixtureDef();
    surfaceFixtureDef->shape = surfaceBox;
    surfaceFixtureDef->density = 0.0f;
    surfaceFixtureDef->friction = 0.5f;

    surfaceBody->CreateFixture(surfaceFixtureDef);


    //Now, give the glass its physics
    glassBodyDef = new b2BodyDef();
    glassBodyDef->position.Set(glass->x(),glass->y());
    glassBodyDef->type = b2_dynamicBody;
    glassBody = world->CreateBody(glassBodyDef);

    //give it collision bounds
    glassShape = new b2PolygonShape();
    glassShape->SetAsBox(10.0f,10.0f);


    //give it qualities such as density and shape.
    glassFixtureDef = new b2FixtureDef();
    glassFixtureDef->shape = glassShape;
    glassFixtureDef->density = 1.0f;

    glassBody->CreateFixture(glassFixtureDef);

    //updates world view
    connect(&timer, &QTimer::timeout, this, &GameLayer::updateWorld);
    timer.start(1000 / 60); // 60 updates per second.

    // connect(this, &GameLayer::heightChanged
    //         , glass, &QGraphicsItem::setPos);
}

void GameLayer::updateWorld()
{
    // Prepare for simulation. Typically we use a time step of 1/60 of a
    // second (60Hz) and 10 iterations. This provides a high quality simulation
    // in most game scenarios.

    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 8;
    int32 positionIterations = 2;

    // Instruct the world to perform a single step of simulation.
    // It is generally best to keep the time step and iterations fixed.
    world->Step(timeStep, velocityIterations, positionIterations);

    // Now print the position and angle of the body.
    b2Vec2 position = glassBody->GetPosition();
    float32 angle = glassBody->GetAngle();
    // box->setPos(position.x - box->rect().width() / 2, -position.y - box->rect().height() / 2); // Convert Box2D coordinates to Qt coordinates
    // box->setRotation(-angle * 180.0 / b2_pi); // Convert Box2D angle to Qt rotation
    emit this->heightChanged((int)position.x* 100,(int)position.y * 100);

}




GameLayer::~GameLayer()
{
	delete ui;
    delete surfaceBodyDef;
    delete surfaceBox;

}

void GameLayer::updateRecipebox(const QString &recipe)
{
	ui->d_DrinkAlgoBox->setText(recipe);
}

void GameLayer::showPauseOverlay()
{
	pauseOverlay->setVisible(true);
}

void GameLayer::hidePauseOverlay()
{
	pauseOverlay->setVisible(false);
}
