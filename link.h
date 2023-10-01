#ifndef LINK_H
#define LINK_H

#include <QGraphicsLineItem>
#include <QGraphicsSimpleTextItem>

class Node;

class Link : public QObject, public QGraphicsLineItem
{
    Q_OBJECT

public:
    Link();
    virtual ~Link(){};

    void addStartingNode(Node *node);
    void addEndingNode(Node *node){endNode = node;}

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    bool sceneEvent(QEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;

    QPointF getMousePos() {return mousePos;}

    Node *getStartNode() const;
    Node *getEndNode() const;

    QGraphicsSimpleTextItem &getTransition() { return transition; }

signals:
    void released();
    void removeMe();

private:
    Node *startNode = nullptr;
    Node *endNode = nullptr;

    QGraphicsSimpleTextItem transition;
    QPointF mousePos;
};

#endif // LINK_H
