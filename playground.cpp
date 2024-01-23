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

    QPixmap map=QIcon(":/Images/vinyle_table_2024_FINAL_V1.svg").pixmap(PLAYGROUND_X,PLAYGROUND_Y);

    setBackgroundBrush(map);

    m_robot.setZValue(3);
    addItem(&m_robot);

    m_previous = m_robot.pos();

    addPots();
    addPlantes();
    addPanneau();
}

Playground::~Playground()
{
}

Robot &Playground::getRobot()
{
    return m_robot;
}

void Playground::addPots()
{
    int y_init[N_STOCK]={508,1282,648,1422,1930,1930};
    int x_init[N_STOCK]={0,0,2930,2930,895,1895};

    int posx_init[N_POTS_PAR_STOCK]={0,70,0,0,70,0};
    int posy_init[N_POTS_PAR_STOCK]={0,35,70,70,105,140};

    float rad[N_STOCK]={0,0,M_PI,M_PI,-M_PI_2,-M_PI_2};

    int posx,posy;

    for(int j=0;j<N_STOCK;j++)
    {
        for (int i=0;i<N_POTS_PAR_STOCK;i++)
        {
            posx=posx_init[i]*qCos(rad[j])-posy_init[i]*qSin(rad[j]);
            posy=posx_init[i]*qSin(rad[j])+posy_init[i]*qCos(rad[j]);
            m_pots[i+j*6]=new Pot(0,QPointF(posx+x_init[j],posy+y_init[j]));
        }
    }

    for (int i=0;i<N_POTS;i++)
    {
        m_pots[i]->setZValue(1);
        addItem(m_pots[i]);
    }
}

void Playground::addPlantes()
{
    srand(time(0));
    int x_init[N_STOCK]={888,888,1888,1888,1388,1388};
    int y_init[N_STOCK]={675,1275,675,1275,475,1475};

    int posx_init[N_POTS_PAR_STOCK]={0,50,50,125,175,125};
    int posy_init[N_POTS_PAR_STOCK]={0,-75,75,-75,0,75};

    for(int j=0;j<N_STOCK;j++)
    {
        int plantes_milieu=0,type;

        int plante_res1=(rand()%3);
        int plante_res2=(rand()%3)+3;

        for(int i=0;i<N_POTS_PAR_STOCK;i++)
        {

            if(i==plante_res1||i==plante_res2)type=2;
            else type=1;
            if(rand()%2&&plantes_milieu==0&&j>3)
            {
                m_plantes[i+j*6]=new Pot(type,QPointF(87+x_init[j],0+y_init[j]));
                plantes_milieu=1;
            }
            else m_plantes[i+j*6]=new Pot(type,QPointF(posx_init[i]+x_init[j],posy_init[i]+y_init[j]));
        }
    }

    for (int i=0;i<N_PLANTES;i++)
    {
        m_plantes[i]->setZValue(2);
        addItem(m_plantes[i]);
    }
}

void Playground::addPanneau()
{
    int posx_init[N_PANNEAUX]={275,500,725,1275,1500,1725,2275,2500,2725};

    for (int i=0;i<N_PANNEAUX;i++)
    {
        m_panneaux[i]=new Panneau(QPointF(posx_init[i],2037));
        m_panneaux[i]->setZValue(2);
        addItem(m_panneaux[i]);
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
    newPos(event->scenePos(), m_robot.pos().theta);
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

        auto distance = parameters["distance"].toInt()/PLAYGROUND_FACTOR * ((parameters["forward"].toBool()) ? -1 : 1);
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

            if (parameters["offset"].toInt() > PLAYGROUND_X/2)
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
            if (parameters["offset"].toInt() > PLAYGROUND_Y/2)
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
    else if (currentNode->getAction()["action"].toString() == "OuvrirStock")
    {
        int mode=m_robot.mode();

        if(parameters["forward"].toBool()==true)
        {
            if(!(mode&0x1))mode+=1;
        }
        else
        {
            if(!(mode&0x2))mode+=2;
        }
        m_robot.setMode(mode);
        newPos(m_robot.pos().coord,m_robot.pos().theta);
    }
    else if (currentNode->getAction()["action"].toString() == "FermerStock")
    {
        int mode=m_robot.mode();

        if(parameters["forward"].toBool()==true)
        {
            if(mode&0x1)mode-=1;
        }
        else
        {
            if(mode&0x2)mode-=2;
        }
        m_robot.setMode(mode);
        newPos(m_robot.pos().coord,m_robot.pos().theta);
    }
    else if (currentNode->getAction()["action"].toString() == "DetecterCouleur")
    {
        if(parameters["forward"].toString()=="FRONT"&&PlantesPrises(0)==3&&PlantesPrises(1)==3)
        {
            m_plantes_prises[0].type=m_plantes[m_plantes_prises[0].number]->getType();
            m_plantes_prises[1].type=m_plantes[m_plantes_prises[0].number]->getType();
            m_plantes_prises[2].type=-abs(m_plantes_prises[0].type-m_plantes_prises[1].type)+2;

            m_plantes_prises[3].type=m_plantes[m_plantes_prises[0].number]->getType();
            m_plantes_prises[4].type=m_plantes[m_plantes_prises[0].number]->getType();
            m_plantes_prises[5].type=-abs(m_plantes_prises[3].type-m_plantes_prises[4].type)+2;
        }
        else if(parameters["forward"].toString()=="BACK"&&PlantesPrises(2)==3&&PlantesPrises(3)==3)
        {
            m_plantes_prises[6].type=m_plantes[m_plantes_prises[0].number]->getType();
            m_plantes_prises[7].type=m_plantes[m_plantes_prises[0].number]->getType();
            m_plantes_prises[8].type=-abs(m_plantes_prises[6].type-m_plantes_prises[7].type)+2;

            m_plantes_prises[9].type=m_plantes[m_plantes_prises[0].number]->getType();
            m_plantes_prises[10].type=m_plantes[m_plantes_prises[0].number]->getType();
            m_plantes_prises[11].type=-abs(m_plantes_prises[9].type-m_plantes_prises[10].type)+2;
        }
    }
    else if (currentNode->getAction()["action"].toString() == "Ventouser")
    {
        int number=parameters["number"].toInt();
        if(!(m_robot.mode()&0x1)&&number<6)
            m_plantes_prises[number].vantouse=1;
        if(!(m_robot.mode()&0x2)&&number>=6)
            m_plantes_prises[number].vantouse=1;
    }
    else if (currentNode->getAction()["action"].toString() == "Deventouser")
    {
        m_plantes_prises[parameters["number"].toInt()].vantouse=0;
    }
    else if (currentNode->getAction()["action"].toString() == "TournerPanneauSolaire")
    {
        int posx_init[N_PANNEAUX]={275,500,725,1275,1500,1725,2275,2500,2725};
        int yoffset=2000-parameters["yoffset"].toInt();
        int angle=parameters["angle"].toInt();
        if(m_robot.pos().coord.y()==yoffset)
        {
            for(int i=0;i<9;i++)
            {
                if(m_robot.pos().coord.x()==posx_init[i])
                {
                    int theta=m_panneaux[i]->theta();
                    m_panneaux[i]->setTheta(theta+angle);
                }
            }
            if(parameters["side"].toString()=="RIGHT")
                newTheta(-90);
            if(parameters["side"].toString()=="LEFT")
                newTheta(90);
        }
    }
}

int Playground::PlantesPrises(int side)
{
    int n=0;
    int offset=side*STOCKAGE_ROBOT/N_STOCK_ROBOT;
    for(int i=offset;i<offset+STOCKAGE_ROBOT/N_STOCK_ROBOT;i++)
    {
        if(m_plantes_prises[i].number!=-1)
            n++;
        if(m_plantes_prises[i-1].number==-1&&i>offset)
        {
            m_plantes_prises[i-1].number=m_plantes_prises[i].number;
            m_plantes_prises[i].number=-1;
        }
    }
    return n;
}

QPointF PointRotate(QPointF pos,int theta)
{
    double rad = (-theta%360 * M_PI) / 180.0;
    int posx=pos.x()*qCos(rad)-pos.y()*qSin(rad);
    int posy=pos.x()*qSin(rad)+pos.y()*qCos(rad);
    return QPointF(posx,posy);
}

void Playground::collisionPlante(int rposx,int rposy,int rtheta,int mode)
{
    QLineF trajectory(m_previous.coord,m_robot.pos().coord);

    int theta=trajectory.angle()-90;

    qDebug()<<"L = "<<trajectory.length();

    bool front=(abs(rtheta-theta))%360<45||(abs(rtheta-theta))%360>315;
    bool back=(abs(rtheta-theta))%360>135&&(abs(rtheta-theta))%360<225;

    static int direction;

    if(front)
        direction=0;
    if(back)
        direction=1;

    QRectF RightZoneRect = m_robot.mapToScene(m_robot.boundingRect().width()/2,m_robot.boundingRect().height()/2,130,-85).boundingRect();
    QRectF LeftZoneRect = m_robot.mapToScene(m_robot.boundingRect().width()/2,m_robot.boundingRect().height()/2,-130,-85).boundingRect();
    QRectF RightTrajectoryRect = m_robot.mapToScene(m_robot.boundingRect().width()/2,m_robot.boundingRect().height()/2,130,trajectory.length()).boundingRect();
    QRectF LeftTrajectoryRect = m_robot.mapToScene(m_robot.boundingRect().width()/2,m_robot.boundingRect().height()/2,-130,trajectory.length()).boundingRect();

    QGraphicsRectItem *RightZone = new QGraphicsRectItem(RightZoneRect);
    QGraphicsRectItem *LeftZone = new QGraphicsRectItem(LeftZoneRect);
    QGraphicsRectItem *RightTrajectory = new QGraphicsRectItem(RightTrajectoryRect);
    QGraphicsRectItem *LeftTrajectory = new QGraphicsRectItem(LeftTrajectoryRect);

    QPen zonePenR(Qt::red);
    QBrush zoneBrushR(Qt::red);

    QPen zonePenL(Qt::blue);
    QBrush zoneBrushL(Qt::blue);

    RightZone->setPen(zonePenR);
    RightZone->setBrush(zoneBrushR);
    RightZone->setTransformOriginPoint(m_robot.pos().coord);
    RightZone->setRotation(-rtheta+direction*180);

    LeftZone->setPen(zonePenL);
    LeftZone->setBrush(zoneBrushL);
    LeftZone->setTransformOriginPoint(m_robot.pos().coord);
    LeftZone->setRotation(-rtheta+direction*180);

    RightTrajectory->setPen(zonePenR);
    RightTrajectory->setBrush(zoneBrushR);
    RightTrajectory->setTransformOriginPoint(m_robot.pos().coord);
    RightTrajectory->setRotation(-rtheta+direction*180);

    LeftTrajectory->setPen(zonePenL);
    LeftTrajectory->setBrush(zoneBrushL);
    LeftTrajectory->setTransformOriginPoint(m_robot.pos().coord);
    LeftTrajectory->setRotation(-rtheta+direction*180);

    qDebug()<<"rtheta = "<<rtheta<<", theta = "<<theta<<", delta = "<<(abs(rtheta-theta))%360;

    if(front)
    {
        addItem(RightZone);
        addItem(LeftZone);
        addItem(RightTrajectory);
        addItem(LeftTrajectory);

        PlantesPrises(0);
        PlantesPrises(1);

        for (int i = 0; i < N_PLANTES; i++)
        {
            if(mode&1)
            {
                if (m_plantes[i]->collidesWithItem(RightZone)||m_plantes[i]->collidesWithItem(RightTrajectory))
                {
                    if(PlantesPrises(0)<3&&m_plantes[i]->zValue()==2)
                    {
                        m_plantes_prises[PlantesPrises(0)].number=i;
                        m_plantes[i]->setZValue(4);
                    }
                }
                if (m_plantes[i]->collidesWithItem(LeftZone)||m_plantes[i]->collidesWithItem(LeftTrajectory))
                {
                    if(PlantesPrises(1)<3&&m_plantes[i]->zValue()==2)
                    {
                        m_plantes_prises[3+PlantesPrises(1)].number=i;
                        m_plantes[i]->setZValue(4);
                    }
                }
            }
        }
        if(mode&2)
        {
            for(int i=6;i<STOCKAGE_ROBOT;i++)
            {
                if(m_plantes_prises[i].number!=-1&&m_plantes_prises[i].vantouse==0&&trajectory.length()>5)
                {
                    m_plantes[m_plantes_prises[i].number]->setZValue(2);
                    m_plantes_prises[i].number=-1;
                }
            }
        }
    }
    if(back)
    {
        addItem(RightZone);
        addItem(LeftZone);
        addItem(RightTrajectory);
        addItem(LeftTrajectory);

        PlantesPrises(2);
        PlantesPrises(3);

        for (int i = 0; i < N_PLANTES; i++)
        {
            if(mode&2)
            {
                if (m_plantes[i]->collidesWithItem(RightZone)||m_plantes[i]->collidesWithItem(RightTrajectory))
                {
                    if(PlantesPrises(2)<3&&m_plantes[i]->zValue()==2)
                    {
                        m_plantes_prises[6+PlantesPrises(2)].number=i;
                        m_plantes[i]->setZValue(4);
                    }
                }
                if (m_plantes[i]->collidesWithItem(LeftZone)||m_plantes[i]->collidesWithItem(LeftTrajectory))
                {
                    if(PlantesPrises(3)<3&&m_plantes[i]->zValue()==2)
                    {
                        m_plantes_prises[9+PlantesPrises(3)].number=i;
                        m_plantes[i]->setZValue(4);
                    }
                }
            }
        }
        if(mode&1)
        {
            for(int i=0;i<STOCKAGE_ROBOT/2;i++)
            {
                if(m_plantes_prises[i].number!=-1&&m_plantes_prises[i].vantouse==0&&trajectory.length()>5)
                {
                    m_plantes[m_plantes_prises[i].number]->setZValue(2);
                    m_plantes_prises[i].number=-1;
                }
            }
        }
    }

    int posx[STOCKAGE_ROBOT]={30,80,55,-80,-30,-55,30,80,55,-80,-30,-55};
    int posy[STOCKAGE_ROBOT]={-85,-85,-128,-85,-85,-128,85,85,128,85,85,128};

    for(int i=0;i<STOCKAGE_ROBOT;i++)
    {
        QPointF pos=PointRotate(QPointF(posx[i],posy[i]),rtheta);
        qDebug()<<m_plantes_prises[i].number;
        if(m_plantes_prises[i].number!=-1)
            m_plantes[m_plantes_prises[i].number]->setPosition(QPointF(rposx+pos.x(),rposy+pos.y()));
    }
}

void Playground::newPos(QPointF pos, double theta, bool first)
{
    QPen pen(Qt::blue, 3);

    clearItems();

    m_robot.setPosition({pos, theta });

    collisionPlante(pos.x(),pos.y(),theta,m_robot.mode());

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
    newPos(pos.coord, theta);
    m_previous = {pos.coord ,theta};
    emit displayNewPos(m_previous);
}

