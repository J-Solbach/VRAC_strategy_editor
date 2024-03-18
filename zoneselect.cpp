#include "zoneselect.h"
#include "node.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QEvent>
#include <QCursor>
#include <QDialog>
#include <QLayout>
#include <QComboBox>
#include "jsonhelperfunctions.h"
#include <QMenu>

ZoneSelect::ZoneSelect(): QObject(), QGraphicsRectItem()
{
    setFlag(QGraphicsItem::ItemIsFocusable);
}


void ZoneSelect::addStartingPoint(QPointF pos)
{
    grabMouse();
    startPos = pos;
    setPos(startPos);
    isMoving=true;
}

void ZoneSelect::addEndingPoint()
{
    grabMouse();
    endPos = mousePos;
    isMoving=false;
}

QRectF ZoneSelect::boundingRect() const
{
    return QGraphicsRectItem::boundingRect();
}

void ZoneSelect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect;

    if (isMoving)
    {
        if(mousePos.x()<0)rect.setX(mousePos.x());
        if(mousePos.y()<0)rect.setY(mousePos.y());
        rect.setWidth(abs(mousePos.x()));
        rect.setHeight(abs(mousePos.y()));
    }
    else
    {
        if(endPos.x()<0)rect.setX(endPos.x());
        if(endPos.y()<0)rect.setY(endPos.x());
        rect.setWidth(abs(endPos.x()));
        rect.setHeight(abs(endPos.y()));
    }

    setRect(rect);
    setPen(QPen(Qt::black,2));
    setBrush(QColor("#555"));
    setOpacity(0.5);
    QGraphicsRectItem::paint(painter, option, widget);
}

void ZoneSelect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    mousePos = event->pos();
    qDebug()<<mousePos;
    update();
    QGraphicsRectItem::mouseMoveEvent(event);
}

void ZoneSelect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsRectItem::mouseReleaseEvent(event);
    mousePos = event->pos();
    ungrabMouse();
    update();
    emit released();
}

bool ZoneSelect::sceneEvent(QEvent *event)
{
    if (event->type() == QEvent::GrabMouse)
    {
        event->accept();
        return true;
    }

    return QGraphicsItem::sceneEvent(event);
}

