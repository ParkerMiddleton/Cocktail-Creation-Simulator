#include "applicationmodel.h"
#include "mainwindow.h"
#include <QPainter>
#include <QtCore>
#include <QtGui>
#include "liquidlayer.h"
#include "liquidmodel.h"
#include "barmodel.h"

LiquidLayer::LiquidLayer(ApplicationModel *app, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LiquidLayer)
{
    // setup
    ui->setupUi(this);
    isDrinkEmpty = false;
    volume = 0;

    hidePauseButton();

    // get bar model from application
    BarModel* bar = app->barModel();

    // setup liquid model
    l = new LiquidModel(this);
    l->setupBox2D();

    // setup timer
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &LiquidLayer::updateSimulation);
    m_timer->start(16); // Update every 16 milliseconds (approximately 60 FPS)

    // overlay pause button
    connect(ui->PauseButton, &QPushButton::clicked,
            app, &ApplicationModel::pause);

    connect(app, &ApplicationModel::newGameStarted,
            this, &LiquidLayer::displayPauseButton);

    connect(app, &ApplicationModel::gameUnpaused,
            this, &LiquidLayer::displayPauseButton);

    connect(app, &ApplicationModel::gamePaused,
            this, &LiquidLayer::hidePauseButton);

    connect(bar, &BarModel::sendVolume,
            this, &LiquidLayer::setVolume);

    connect(bar, &BarModel::sendDrinkName,
            this, &LiquidLayer::setDrinkColor);

    connect(bar, &BarModel::informEmptyDrink,
            this, &LiquidLayer::emptyDrink);

    // setup colors for each drink
    drinkColors["whiskey"] = QColor(165, 113, 10); // Whiskey color
    drinkColors["tequila"] = QColor(240, 230, 14); // Tequila color
    drinkColors["rum"] = QColor(139, 69, 19);       // Rum color
    drinkColors["vodka"] = QColor(240, 255, 13);   // Vodka color
    drinkColors["gin"] = QColor(173, 255, 30);      // Gin color
    drinkColors["kahlua"] = QColor(70, 40, 0);  //kahlua

}

void LiquidLayer::displayPauseButton() {
    ui->PauseButton->setVisible(true);
}

void LiquidLayer::hidePauseButton() {
    ui->PauseButton->setVisible(false);
}

void LiquidLayer::setVolume(int v) {
    isDrinkEmpty = false;
    l->addLiquid(v);
}

void LiquidLayer::emptyDrink() {
    l->clearDrink();
    isDrinkEmpty = true;
    emit emptyLiquid();
}

void LiquidLayer::setDrinkColor(QString drinkName) {
    currentDrink = drinkName;
}

LiquidLayer::~LiquidLayer()
{
    // Any cleanup code for LiquidPoor goes here
    // For example, deallocating dynamically allocated resources
    delete ui; // If ui is dynamically allocated
}


void LiquidLayer::updateSimulation()
{
    // Step the Box2D simulation
    l->m_world->Step(1.0f / 60.0f, 1, 3);

    update(); // Trigger repaint
}


void LiquidLayer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // THIS CODE IS FOR VISUALIZING THE PHYSICS BOUNDARIES BY DRAWING THEM.

    // Draw the glass container
    // const b2Body* containerBody = l->m_world->GetBodyList();
    // while (containerBody) {
    //     // Check if the body is the container
    //     if (containerBody->GetType() == b2_staticBody) {
    //         // Draw the container
    //         b2Fixture* fixture = const_cast<b2Fixture*>(containerBody->GetFixtureList());
    //         while (fixture) {
    //             if (fixture->GetType() == b2Shape::e_polygon) {
    //                 b2PolygonShape* shape = dynamic_cast<b2PolygonShape*>(fixture->GetShape());
    //                 if (shape) {
    //                     QPainterPath path;
    //                     for (int i = 0; i < shape->GetVertexCount(); ++i) {
    //                         b2Vec2 vertex = containerBody->GetWorldPoint(shape->GetVertex(i));
    //                         if (i == 0)
    //                             path.moveTo(vertex.x, vertex.y);
    //                         else
    //                             path.lineTo(vertex.x, vertex.y);
    //                     }
    //                     path.closeSubpath();
    //                     painter.setPen(Qt::black);
    //                     painter.setBrush(Qt::white); // Set the color of the container
    //                     painter.drawPath(path);
    //                 }
    //             }
    //             fixture = const_cast<b2Fixture*>(fixture->GetNext());
    //         }
    //     }
    //     containerBody = containerBody->GetNext();
    // }

    // Get the particle system list from the world
    const b2ParticleSystem* particleSystemList = l->m_world->GetParticleSystemList();

    // Retrieve the color corresponding to the current drink
    QColor ingredientColor = drinkColors[currentDrink];

    // Iterate through all particle systems
    for (const b2ParticleSystem* particleSystem = particleSystemList; particleSystem; particleSystem = particleSystem->GetNext()) {
        // Iterate through all particles in the current particle system
        for (int i = 0; i < particleSystem->GetParticleCount(); ++i) {
            // Get particle position
            b2Vec2 particlePosition = particleSystem->GetPositionBuffer()[i];
            painter.setPen(ingredientColor);
            painter.setBrush(ingredientColor);
            painter.drawEllipse(QPointF(particlePosition.x, particlePosition.y), 2, 2); // Adjust the size as needed
        }
    }
}
