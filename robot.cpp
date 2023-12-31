#include "robot.h"

Robot::Robot()
{
    m_pixmap = ":/Images/robot_top.png";
    QPixmap *robot = new QPixmap(m_pixmap);

    m_pos.coord = QPointF(300/5, 300/5);
    m_pos.theta = 0;

    this->setPixmap(robot->transformed(QTransform().rotate(- m_pos.theta)));
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
    this->setPixmap(robot->transformed(QTransform().rotate(- m_pos.theta)));
}

const position &Robot::pos() const
{
    return m_pos;
}
