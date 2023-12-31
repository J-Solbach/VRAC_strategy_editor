#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QJsonObject>
#include <QJsonArray>
#include <link.h>

class Node : public QGraphicsTextItem
{
    Q_OBJECT
public:
    Node(QString tag, QJsonObject json, QGraphicsItem *parent = nullptr);
    virtual ~Node() { links.clear();};

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QPointF getOutPort(){return outPort.center();}
    QPointF getInPort(){return inPort.center();}

    void addLink(Link* link);
    void removeLink(Link* link);

    void setupName();

    QJsonValue getAction();

    const QVector<Link *> &getLinks() const;

signals:
    void outPortClicked();
    void inPortReleased();
    void removeMe();
    void copied();
    void selected();
    void nameChanged(QString);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QJsonValue action;
    QVector<Link*> links;

    QString m_tag;

    QPointF startPos;
    QPointF edgeStartPos;
    QPointF edgeEndPos;

    QRectF outPort;
    QRectF inPort;

    bool isMoving;
    bool isCreatingEdge;
    bool isHovered;
};

#endif // NODE_H
