#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include <QGraphicsScene>
#include "robot.h"
#include "pot.h"
#include "node.h"

#define PLAYGROUND_X 3000
#define PLAYGROUND_Y 2000

#define PLAYGROUND_FACTOR 1

#define N_POTS 36
#define N_PLANTES 36
#define N_STOCK 6
#define N_POTS_PAR_STOCK 6

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

    void clearItems();

signals:
    void displayNewPos(position pos);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

public slots:

    void setCurrentDisplayedNode(Node *currentNode);

    void newPos(QPointF goal, double theta, bool first = false);

    void newPosBezier(QPointF goal, double theta, int16_t radius1, int16_t radius2);

    void newTheta(double theta);

private:
    Robot m_robot;
    Pot *m_pots[N_POTS];
    Pot *m_plantes[N_PLANTES];
    position m_previous;
};

#endif // PLAYGROUND_H
