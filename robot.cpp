#include "robot.h"

Robot::Robot()
{
    m_pixmap = ":/Images/robot_top.png";
    QPixmap *robot = new QPixmap(m_pixmap);

    m_pos.coord = QPointF(0,1575);
    m_pos.theta = 0;

    // TODO : demander un png du robot à taille reelle (1 pixel = 1mm)
    setPos(QPointF(m_pos.coord.x() - boundingRect().width()/2 ,m_pos.coord.y() - boundingRect().height()/2));
    this->setPixmap(robot->transformed(QTransform().scale(6.9,6.9).rotate(- m_pos.theta+90)));
}


void Robot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsPixmapItem::paint(painter,option, widget);
}

QRectF Robot::boundingRect() const
{
    return QRectF(pixmap().rect());
}

void Robot::setPosition(position pos)
{
    m_pos = pos;

    setPos(QPointF(m_pos.coord.x() - boundingRect().width()/2 ,m_pos.coord.y() - boundingRect().height()/2));
    QPixmap *robot = new QPixmap(m_pixmap);
    this->setPixmap(robot->transformed(QTransform().scale(6.9,6.9).rotate(- m_pos.theta+90)));
}

const position &Robot::pos() const
{
    return m_pos;
}
