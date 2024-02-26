#include "robot.h"

Robot::Robot()
{
    m_pixmap = ":/Images/robot_ferme.png";
    QPixmap *robot = new QPixmap(m_pixmap);

    m_pos.coord = QPointF(225, 225);
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

void Robot::setEtat(enum mode frontStock,enum mode backStock,bool reset)
{
    enum type_etat{debut,close,frontOpen,backOpen,open,fin};
    static enum type_etat etat=debut;
    m_frontStock = frontStock;
    m_backStock = backStock;

    for(int i=0;i<2;i++)
    {
        switch (etat) {
        case debut:
            m_pixmap = ":/Images/robot_ferme.png";
            if(m_frontStock==Open)etat=frontOpen;
            if(m_backStock==Open)etat=backOpen;
            break;

        case close:
            m_pixmap = ":/Images/robot_ferme.png";
            if(m_frontStock==Open)etat=frontOpen;
            if(m_backStock==Open)etat=backOpen;
            if(reset)etat=debut;
            break;

        case frontOpen:
            m_pixmap = ":/Images/ouvert_devant.png";
            if(m_frontStock==Close)etat=close;
            if(m_backStock==Open)etat=open;
            break;

        case backOpen:
            m_pixmap = ":/Images/ouvert_derriere.png";
            if(m_frontStock==Open)etat=open;
            if(m_backStock==Close)etat=close;
            if(reset)etat=debut;
            break;

        case open:
            m_pixmap = ":/Images/robot_ouvert.png";
            if(m_frontStock==Close)etat=backOpen;
            if(m_backStock==Close)etat=frontOpen;
            if(reset)etat=debut;
            break;

        default:
            m_pixmap = ":/Images/robot_ferme.png";
            break;
        }
    }
}

mode Robot::frontStock()
{
    return m_frontStock;
}

mode Robot::backStock()
{
    return m_backStock;
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
