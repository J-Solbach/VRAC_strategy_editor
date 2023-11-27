#include "playground.h"
#include "robot.h"
#include <QGraphicsSceneMouseEvent>
#include <QSvgRenderer>
#include <QPainter>
#include <QGraphicsPathItem>
#include <QIcon>

Playground::Playground()
{
    setSceneRect(QRectF(0, 0, (PLAYGROUND_X), PLAYGROUND_Y));

    QPixmap map=QIcon(":/Images/vinyle_table_2024_FINAL_V1.svg").pixmap(3000,2000);

    setBackgroundBrush(map);

    m_robot.setZValue(3);
    addItem(&m_robot);
    addPotFer();
    addPot();
}

Playground::~Playground()
{
}

Robot &Playground::getRobot()
{
    return m_robot;
}

void Playground::addPotFer()
{
    int y_init[6]={508,1282,508,1282,1930,1930};
    int x_init[6]={0,0,2930,2930,895,1895};

    int inv[6]={1,1,-1,-1,-1,-1};

    for(int j=0;j<6;j++)
    {
        for (int i=0;i<5;i++)
        {
            m_pot_fer[i+j*6]=new Pot(0,QPointF(inv[j]*(i&0x1)*70+x_init[j],(140*i/4)+y_init[j]));
            if(j>3)m_pot_fer[i+j*6]=new Pot(0,QPointF((140*i/4)+x_init[j],inv[j]*(i&0x1)*70+y_init[j]));
        }
        m_pot_fer[5+j*6]=new Pot(0,QPointF(x_init[j],70+y_init[j]));
        if(j>3)m_pot_fer[5+j*6]=new Pot(0,QPointF(70+x_init[j],y_init[j]));
    }

    for (int i=0;i<36;i++)
    {
        m_pot_fer[i]->setZValue(1);
        addItem(m_pot_fer[i]);
    }
}

void Playground::addPot()
{
    srand(time(0));
    int x_init[6]={888,888,1888,1888,1388,1388};
    int y_init[6]={675,1275,675,1275,475,1475};

    int posx_init[6]={0,50,125,175,125,50};
    int posy_init[6]={0,-75,-75,0,75,75};

    for(int j=0;j<6;j++)
    {
        int n=0,k=0,type;
        for(int i=0;i<6;i++)
        {
            type=(rand()%2)+1;
            if(n>=2)type=1;
            else if(i>3)type=2;
            if(rand()%2&&k==0&&j>3)
            {
                m_pot[i+j*6]=new Pot(type,QPointF(87+x_init[j],0+y_init[j]));
                k=1;
            }
            else m_pot[i+j*6]=new Pot(type,QPointF(posx_init[i]+x_init[j],posy_init[i]+y_init[j]));
            if(type==2)n++;
        }
    }

    for (int i=0;i<36;i++)
    {
        m_pot[i]->setZValue(2);
        addItem(m_pot[i]);
    }
}

void Playground::clearItems()
{
    QList<QGraphicsItem*> all = items();
    for (int i = 0; i < all.size(); i++)
    {
        QGraphicsItem *gi = all[i];

        if (gi->zValue()==0)
            removeItem(gi);
    }

}

void Playground::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit displayNewPos({event->scenePos(), m_robot.pos().theta});
    m_robot.setPosition({event->scenePos(), m_robot.pos().theta});
    qDebug() << "emitting new coord";

    QGraphicsScene::mouseMoveEvent(event);
}

void Playground::setCurrentDisplayedNode(Node *currentNode)
{
    auto parameters = currentNode->getAction()["parameters"].toObject();

    if (currentNode->getAction()["action"].toString() == "XYT"
     || currentNode->getAction()["action"].toString() == "SetOdometry"
     || currentNode->getAction()["action"].toString() == "Absolute")
    {
        auto x = parameters["x"].toInt()/PLAYGROUND_FACTOR;
        auto y = parameters["y"].toInt()/PLAYGROUND_FACTOR;
        auto theta = parameters["theta"].toDouble();

        auto withRotate = (currentNode->getAction()["action"].toString() == "XYT") ? parameters["withRotate"].toBool() : true;
        auto forward = parameters["forward"].toBool();

        QPointF pos(x,y);

        if (!withRotate)
        {
            QLineF trajectoryLine(m_previous.coord, pos);

            theta = trajectoryLine.angle()-90;

            if (!forward) theta -= 180;

            qDebug() << m_previous.coord << pos << "calculated theta : " << theta;
        }

        newPos(pos, theta);
    }
    else if (currentNode->getAction()["action"].toString() == "Line")
    {
        double rad = (m_previous.theta * M_PI) / 180.0;

        auto distance = parameters["distance"].toInt()/PLAYGROUND_FACTOR * ((parameters["forward"].toBool()) ? 1 : -1);
        auto x = m_previous.coord.x() + (distance * qSin(rad));
        auto y = m_previous.coord.y() + (distance * qCos(rad));

        newPos(QPointF(x, y), m_previous.theta);
    }
    else if (currentNode->getAction()["action"].toString() == "Rotate")
    {
        auto theta = parameters["theta"].toDouble() + ((parameters["relative"].toBool()) ? m_previous.theta : 0);

        newTheta(theta);
    }
    else if (currentNode->getAction()["action"].toString() == "Bezier")
    {
        auto x = parameters["x"].toInt()/PLAYGROUND_FACTOR;
        auto y = parameters["y"].toInt()/PLAYGROUND_FACTOR;
        auto t = parameters["theta"].toDouble();
        auto radius1 = parameters["radius1"].toInt()/PLAYGROUND_FACTOR;
        auto radius2 = parameters["radius2"].toInt()/PLAYGROUND_FACTOR;

        newPosBezier(QPointF(x,y), t, radius1, radius2);
    }
    else if (currentNode->getAction()["action"].toString() == "Homing")
    {
        auto axis = parameters["axis"].toBool();

        auto x = (axis == true) ? parameters["offset"].toInt()/PLAYGROUND_FACTOR : m_previous.coord.x();
        auto y = (axis == false) ? - parameters["offset"].toInt()/PLAYGROUND_FACTOR + PLAYGROUND_Y : m_previous.coord.y();

        double theta;
        if (axis)
        { // HOMING de X

            if (parameters["offset"].toInt() > 1500)
            {
                theta = (parameters["forward"].toBool()) ? 90.0 : -90.0;
            }
            else
            {
                 theta = (parameters["forward"].toBool()) ? -90.0 : 90.0;
            }
        }
        else
        {
            if (parameters["offset"].toInt() > 1000)
            {
                theta = (parameters["forward"].toBool()) ? 0 : 180.0;
            }
            else
            {
                theta = (parameters["forward"].toBool()) ? 180.0 : 0;
            }
        }

        newPos(QPointF(x,y), theta);
    }
}

void Playground::newPos(QPointF pos, double theta, bool first)
{
    QPen pen(Qt::blue, 3);

    clearItems();

    m_robot.setPosition({pos, theta });

    if (!first)
    {
        QGraphicsLineItem *trajectory = new QGraphicsLineItem(QLineF(m_previous.coord, pos));
        trajectory->setPen(pen);
        addItem(trajectory);
    }

    m_previous = {pos, theta};
    emit displayNewPos(m_previous);
}

void Playground::newPosBezier(QPointF goal, double theta, int16_t radius1, int16_t radius2)
{
    QPen pen(Qt::blue, 3);

    clearItems();

    double rad1 = (m_previous.theta * M_PI) / 180.0;
    double rad2 = (theta * M_PI) / 180.0;

    QPointF checkPoint1;
    checkPoint1.setX(m_previous.coord.x() + (radius1 * qSin(rad1)));
    checkPoint1.setY(m_previous.coord.y() + (radius1 * qCos(rad1)));

    QPointF checkPoint2;
    checkPoint2.setX(goal.x() + (radius2 * qSin(rad2)));
    checkPoint2.setY(goal.y() + (radius2 * qCos(rad2)));

    QPainterPath bezier;
    bezier.moveTo(m_previous.coord);
    bezier.cubicTo(checkPoint1, checkPoint2, goal);

    QGraphicsPathItem *bezierItem = new QGraphicsPathItem(bezier);
    addItem(bezierItem);
    bezierItem->setPen(pen);

    QGraphicsLineItem *trajectory = new QGraphicsLineItem(QLineF(m_previous.coord, checkPoint1));
    addItem(trajectory);

    QGraphicsLineItem *trajectory2 = new QGraphicsLineItem(QLineF(goal, checkPoint2));
    addItem(trajectory2);

    m_robot.setPosition({goal,  theta});

    qDebug() << theta;

    m_previous = {goal ,theta};
    emit displayNewPos(m_previous);
}

void Playground::newTheta(double theta)
{
    position pos = m_robot.pos();
    m_robot.setPosition({pos.coord, theta });
    m_previous = {pos.coord ,theta};
    emit displayNewPos(m_previous);
}

