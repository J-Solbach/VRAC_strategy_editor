#ifndef TOOLBOXSCENE_H
#define TOOLBOXSCENE_H

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

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    void addNode(Node *node);
    void addLink(Link *link);

    const QVector<Node *> &getNodes() const;

    Node * getNode(QString tag);

    const QVector<Link *> &getLinks() const;

private slots:

    void startLink();
    void endLink();

    void removeNode();
    void removeLink();

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

    Node *copiedNode = nullptr;
};

#endif // TOOLBOXSCENE_H
