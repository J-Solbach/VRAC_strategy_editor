#include "meta_action.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QGridLayout>
#include <QToolButton>
#include <QFile>
#include <QDir>
#include <QDebug>
#include "node.h"
#include <QFileDialog>
#include <QFileInfo>
#include "jsonhelperfunctions.h"

meta_action::meta_action(QString fileName) :m_fileName(fileName){}

void  meta_action::setMetaActionScene(ToolBoxScene *stratBuilder)
{
    stratBuilder->clearScene();

    QFile f(m_fileName);

    f.open(QIODevice::ReadOnly);

    QJsonDocument doc = QJsonDocument::fromJson(QString(f.readAll()).toUtf8());
    QJsonObject rootTest = doc.object();

    QJsonArray actions = rootTest["actions"].toArray();

    auto action = actions.first().toObject();

    for (auto actionRef : actions)
    {
        auto action = actionRef.toObject();
        Node *testNode = new Node(action["tag"].toString(), action);
        stratBuilder->addNode(testNode);
        stratBuilder->update();
    }

    //setupLinks
    for (auto actionRef : actions)
    {
        auto action = actionRef.toObject();

        Node *startNode = stratBuilder->getNode(action["tag"].toString());

        if (startNode== nullptr) continue;

        QJsonArray transitions = action["transitions"].toArray();

        for (auto transitionRef : transitions)
        {
            auto transition = transitionRef.toObject();
            Node *endNode = stratBuilder->getNode(transition["destination"].toString());

            if (endNode != nullptr)
            {
                Link *newLink = new Link();

                newLink->addStartingNode(startNode);
                startNode->addLink(newLink);
                newLink->addEndingNode(endNode);
                newLink->getTransition().setText(transition["type"].toString());

                stratBuilder->addLink(newLink);
            }
        }
    }

    Node * currentNode = stratBuilder->getNodes().first();
    QPointF currentPos(stratBuilder->sceneRect().center().x(), -40);

    stratBuilder->organizeScene(currentNode, currentPos);

    stratBuilder->update();
}
