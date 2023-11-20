#include "pot.h"

Pot::Pot(int type,QPointF pos):m_type(type),m_pos(pos)
{
    if(m_type==0)m_pixmap = ":/Images/pot-removebg-preview.png";
    if(m_type==1)m_pixmap = ":/Images/pot.png";
    if(m_type==2)m_pixmap = ":/Images/potRes.png";

    QPixmap *pot =new QPixmap(m_pixmap);

    setPos(m_pos);

    this->setPixmap(pot->transformed(QTransform().scale(1,1)));
}


void Pot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsPixmapItem::paint(painter,option, widget);
}

QRectF Pot::boundingRect() const
{
    return QRectF(pixmap().rect());
}

void Pot::setPosition(QPointF pos)
{
    m_pos = pos;

    setPos(QPointF(m_pos.x() - boundingRect().width()/2 ,m_pos.y() - boundingRect().height()/2));
    QPixmap *pot =new QPixmap(m_pixmap);
    this->setPixmap(pot->transformed(QTransform().scale(1,1)));
}

const QPointF &Pot::pos() const
{
    return m_pos;
}

