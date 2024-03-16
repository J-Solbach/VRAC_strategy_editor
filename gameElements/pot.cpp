#include "pot.h"

Pot::Pot(int type,QPointF pos):m_pos(pos),m_type(type)
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

void Pot::setPosition(QPointF pos,bool reset)
{
    m_pos = pos;
    if(reset)
        setPos(m_pos);
    else
        setPos(QPointF(m_pos.x() - boundingRect().width()/2 ,m_pos.y() - boundingRect().width()/2 ));
    QPixmap *pot =new QPixmap(m_pixmap);
    this->setPixmap(pot->transformed(QTransform().scale(1,1)));
}

int Pot::getType()
{
    return m_type;
}

const QPointF &Pot::pos() const
{
    return m_pos;
}

