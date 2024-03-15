#ifndef NODE_H
#define NODE_H

#include "toolboxscene.h"
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

    void setPreviousStartNode(QString previousStartNode);
    QString getPreviousStartNode(){return m_previousStartNode;}

    bool isMetaAction();
    QString getfileName(){return "D:/travail/CRAC/Coupe2024/metaActions/"+action["file"].toString()+".json";}

    void setupName();

    QJsonValue getAction();

    ToolBoxScene *getMetaScene(){return stratBuilder;}

    const QVector<Link *> &getLinks() const;

signals:
    void outPortClicked();
    void inPortReleased();
    void removeMe();
    void copied();
    void selected();
    void MetaActionSelected(QString);
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

    ToolBoxScene *stratBuilder;

    int m_NbEndLinks=0;

    QString m_tag;
    QString m_previousStartNode=nullptr;

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
