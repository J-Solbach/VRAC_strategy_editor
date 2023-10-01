#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include <QGraphicsScene>
#include "shared_types.hpp"
#include "robot.h"
#include "node.h"

class Robot;

class Playground : public QGraphicsScene
{
    Q_OBJECT
public:
    Playground();
    virtual ~Playground();

    Robot &getRobot();

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
    position m_previous;
};

#endif // PLAYGROUND_H
