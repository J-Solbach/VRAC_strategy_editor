#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include <QGraphicsScene>
#include "robot.h"
#include "gameElements/pot.h"
#include "gameElements/panneau.h"
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
#define N_JARDINIERE 6

struct plantes
{
    int indice=-1;
    bool ventouse=0;
};

class Robot;

class Playground : public QGraphicsScene
{
    Q_OBJECT
public:
    Playground();
    virtual ~Playground();

    Robot &getRobot();

    void resetItems();

    void addPots();

    void addPlantes();

    void addPanneau();

    void setJardiniere();

    int PlantesPrises(int side);

    void collisionPlante(int rposx, int rposy, int rtheta);

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

    position m_robot_init;
    position m_previous;

    Pot *m_pots[N_POTS];
    Pot *m_plantes[N_PLANTES];

    QPointF m_pots_init[N_POTS];
    QPointF m_plantes_init[N_PLANTES];

    Panneau *m_panneaux[N_PANNEAUX];
    plantes m_plantes_prises[STOCKAGE_ROBOT];

    QGraphicsRectItem *m_jardiniere[N_JARDINIERE];
};

#endif // PLAYGROUND_H
