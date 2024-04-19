#ifndef LIQUID_H
#define LIQUID_H

#include <QMainWindow>
#include <Box2D/Box2D.h>

#include <QObject> // Include QObject for signal-slot mechanism

class LiquidModel : public QObject {
    Q_OBJECT // Macro needed for signal-slot mechanism


public slots:
    void addLiquid(int volume);

public:
    b2ParticleSystemDef particleSystemDef;
    b2ParticleSystem* particleSystem;
    void setupLiquidParticleSystem();
    explicit LiquidModel(QWidget *parent = nullptr); // Constructor with parent widget
    void clearDrink();
    ~LiquidModel();
    LiquidModel();
    void setupBox2D();
    b2World* m_world;
    std::vector<b2ParticleSystem*> particleSystemsList;
};

#endif // LIQUID_H
