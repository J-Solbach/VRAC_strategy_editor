#ifndef TOOLBOXSCENE_H
#define TOOLBOXSCENE_H

#include "zoneselect.h"
#include <QGraphicsScene>
#include <QDragMoveEvent>
#include <QVector>

class Node;
class Link;

class ToolBoxScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit ToolBoxScene(QObject *parent = nullptr);
    virtual ~ToolBoxScene();

    void clearScene();

    void organizeScene(Node *currentNode, QPointF currentPos);
    void organize_MetaAction(QString fileName);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    void addNode(Node *node);
    void addLink(Link *link);

    const QVector<Node *> &getNodes() const;

    Node * getNode(QString tag);
    Node *lastNode();

    int countLink(Node *selectedNode);

    const QVector<Link *> &getLinks() const;

private slots:

    void endZone();

    void startLink();
    void endLink();

    void removeNode();
    void removeLink();

    void nodeIsMoving(QPointF delta);
    void nodeIsCopied();
    void nodeSelected();

    void MetaActionSelected(QString fileName);

    void checkNaming(QString);

signals:
    void displayStep(Node *);
    void Load_MetaAction(QString);
private:
    QVector<Node*>nodes;
    QVector<Link*>links;

    ZoneSelect *zone = nullptr;

    Node *copiedNode = nullptr;
    Node *m_lastNode = nullptr;

    QPointF startPos;
};

#endif // TOOLBOXSCENE_H
