#include "panneau.h"

Panneau::Panneau(QPointF pos):m_pos(pos)
{
    m_pixmap = ":/Images/panneau_solaire.png";

    QPixmap *panneau =new QPixmap(m_pixmap);

    this->setPixmap(panneau->transformed(QTransform().rotate(-m_theta)));
    setPos(QPointF(m_pos.x() - boundingRect().width()/2 ,m_pos.y() - boundingRect().width()/2 ));
}


void Panneau::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsPixmapItem::paint(painter,option, widget);
}

QRectF Panneau::boundingRect() const
{
    return QRectF(pixmap().rect());
}

void Panneau::setTheta(int theta)
{
    m_theta=theta;

    QPixmap *panneau =new QPixmap(m_pixmap);

    this->setPixmap(panneau->transformed(QTransform().rotate(-m_theta)));
    setPos(QPointF(m_pos.x() - boundingRect().width()/2 ,m_pos.y() - boundingRect().width()/2 ));
}

int Panneau::theta() const
{
    return m_theta;
}

