#ifndef ZONESELECT_H
#define ZONESELECT_H

#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>

class Node;

class ZoneSelect:public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    ZoneSelect();
    virtual ~ZoneSelect(){};

    void addStartingPoint(QPointF pos);
    QPointF addEndingPoint();

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    bool sceneEvent(QEvent *event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    QPointF getMousePos() {return mousePos;}

signals:
    void released();
    void removeMe();

private:

    QPointF mousePos;
    QPointF startPos;
    QPointF endPos;

    int width;
    int height;

    bool isMoving;
};

#endif // ZONESELECT_H
