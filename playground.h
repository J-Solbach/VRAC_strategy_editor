#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include <QGraphicsScene>
#include "robot.h"
#include "pot.h"
#include "panneau.h"
#include "node.h"

#define PLAYGROUND_X 3000
#define PLAYGROUND_Y 2000

#define PLAYGROUND_FACTOR 1

#define N_POTS 36
#define N_PLANTES 36
#define N_STOCK 6
#define N_POTS_PAR_STOCK 6
#define STOCKAGE_ROBOT 12
#define N_STOCK_ROBOT 4
#define N_PANNEAUX 9

struct plantes
{
    int number=-1;
    int type=0;
    bool vantouse=0;
};

class Robot;

class Playground : public QGraphicsScene
{
    Q_OBJECT
public:
    Playground();
    virtual ~Playground();

    Robot &getRobot();

    void addPots();

    void addPlantes();

    void addPanneau();

    int PlantesPrises(int side);

    void collisionPlante(int rposx, int rposy, int rtheta, int mode);

    void clearItems();

signals:
    void displayNewPos(position pos);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

public slots:

    void setCurrentDisplayedNode(Node *currentNode);

    void newPos(QPointF pos, double theta, bool first = false);

    void newPosBezier(QPointF goal, double theta, int16_t radius1, int16_t radius2);

    void newTheta(double theta);

private:
    Robot m_robot;
    Pot *m_pots[N_POTS];
    Pot *m_plantes[N_PLANTES];
    Panneau *m_panneaux[N_PANNEAUX];
    plantes m_plantes_prises[STOCKAGE_ROBOT];
    position m_previous;
};

#endif // PLAYGROUND_H
