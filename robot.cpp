#include "robot.h"

Robot::Robot()
{
    m_pixmap = ":/Images/robot_ferme.png";
    QPixmap *robot = new QPixmap(m_pixmap);

    m_pos.coord = QPointF(225, 1775);
    m_pos.theta =-90;

    // TODO : demander un png du robot à taille reelle (1 pixel = 1mm)

    this->setPixmap(robot->transformed(QTransform().rotate(-m_pos.theta)));
    setPos(QPointF(m_pos.coord.x() - boundingRect().width()/2 ,m_pos.coord.y() - boundingRect().height()/2));
}


void Robot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsPixmapItem::paint(painter,option, widget);
}

QRectF Robot::boundingRect() const
{
    return QRectF(pixmap().rect());
}

void Robot::setMode(int mode)
{
    m_mode=mode;

    switch (m_mode) {
    case 0:
        m_pixmap = ":/Images/robot_ferme.png";
        break;

    case 1:
        m_pixmap = ":/Images/ouvert_devant.png";
        break;

    case 2:
        m_pixmap = ":/Images/ouvert_derriere.png";
        break;

    case 3:
        m_pixmap = ":/Images/robot_ouvert.png";
        break;

    default:
        m_pixmap = ":/Images/robot_ferme.png";
        break;
    }
}

int Robot::mode()
{
    return m_mode;
}

void Robot::setPosition(position pos)
{
    m_pos = pos;
    QPixmap *robot = new QPixmap(m_pixmap);

    this->setPixmap(robot->transformed(QTransform().rotate(-m_pos.theta)));
    setPos(QPointF(m_pos.coord.x() - boundingRect().width()/2 ,m_pos.coord.y() - boundingRect().height()/2));
}

const position &Robot::pos() const
{
    return m_pos;
}
