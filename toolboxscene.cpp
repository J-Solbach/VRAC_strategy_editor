#include "toolboxscene.h"
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMimeData>
#include <QIODevice>
#include <QSvgRenderer>
#include <QPainter>
#include <node.h>
#include <link.h>
#include <algorithm>

ToolBoxScene::ToolBoxScene(QObject *parent) : QGraphicsScene(parent)
{
    setSceneRect(QRectF(0, 0, 900, 900));

    setBackgroundBrush(QColor("#666"));
}

ToolBoxScene::~ToolBoxScene(){}

void ToolBoxScene::clearScene()
{
    for (auto node : nodes)
    {
        links.erase(std::remove_if(links.begin(), links.end(), [&](Link *link)
        {
            if ((link->getStartNode() == node) || (link->getEndNode() == node))
            {
                link->getEndNode()->removeEndLink();
                link->getStartNode()->removeLink(link);
                delete link;
                link = nullptr;
                return true;
            }
            return false;
        }), links.end());

        delete node;
    }

    nodes.clear();
}

void ToolBoxScene::organizeScene(Node *currentNode, QPointF currentPos)
{
    qDebug() << "organize" << currentNode->toPlainText();

    currentPos.setY(currentPos.y() + 55);
    currentPos.setX(currentPos.x() - currentNode->boundingRect().width()/2);

    currentNode->setPos(currentPos);
    currentNode->setupName();

    int alt = 1;
    int count = 2;

    int countToPlace = 0;

    for (auto link : currentNode->getLinks())
    {
        countToPlace++;
    }

    if (countToPlace%2)
    {
        alt = 0;
    }

    for (auto link : currentNode->getLinks())
    {
        int realDist = (count/2) * 150;

        currentPos.setX(currentNode->scenePos().x() + currentNode->boundingRect().width()/2  + (alt * realDist));

        if (currentPos.x() > (this->width() - currentNode->boundingRect().width()/2))
        {
            currentPos.setX(this->width() - currentNode->boundingRect().width()/2);
        }

        organizeScene(link->getEndNode(), currentPos);

        if (alt == 0)
        {
            alt = 1;
        }
        else
        {
            alt *=-1;
            count++;
        }
    }
}

void ToolBoxScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
}

void ToolBoxScene::keyPressEvent(QKeyEvent *event)
{
    if (event->matches(QKeySequence::Paste))
    {
        if( copiedNode == nullptr) return;

        Node *newNode = new Node(copiedNode->toPlainText(), copiedNode->getAction().toObject());
        addNode(newNode);
        newNode->setupName();
    }

    else
    {
        QGraphicsScene::keyPressEvent(event);
    }
}

void ToolBoxScene::addNode(Node *node)
{
    addItem(node);
    QPointF previousPos;
    uint8_t offset = 10;

    if (nodes.isEmpty())
    {
        previousPos = sceneRect().center();
        previousPos.setY(0);
        previousPos.setX(previousPos.x() - node->boundingRect().width()/2);
        offset = 20;
    }
    else
    {
        previousPos = nodes.last()->pos();

    }

    QPointF newPos = sceneRect().center();
    newPos.setY(previousPos.y() + offset);
    newPos.setX(newPos.x() - node->boundingRect().width()/2);

    node->setPos(newPos);
    nodes.push_back(node);

    connect(node, &Node::outPortClicked, this, &ToolBoxScene::startLink);
    connect(node, &Node::removeMe, this,  &ToolBoxScene::removeNode);
    connect(node, &Node::copied, this, &ToolBoxScene::nodeIsCopied);
    connect(node, &Node::selected, this, &ToolBoxScene::nodeSelected);
    connect(node, &Node::nameChanged, this, &ToolBoxScene::checkNaming);
}

void ToolBoxScene::addLink(Link *link)
{
    addItem(link);
    links.push_back(link);

    connect(link, &Link::removeMe, this, &ToolBoxScene::removeLink);
}


void ToolBoxScene::startLink()
{
    Node *node = static_cast<Node*>(sender());

    qDebug() << "Creating link for node :" << node->toPlainText();
    Link *newLink = new Link();
    addItem(newLink);
    newLink->addStartingNode(node);

    connect(newLink, &Link::released, this, &ToolBoxScene::endLink);

    links.push_back(newLink);
}

void ToolBoxScene::endLink()
{
    Link *newLink = static_cast<Link*>(sender());

    auto itNode = std::find_if(nodes.begin(), nodes.end(), [&](Node *node)
    {
        return (node->collidesWithItem(newLink) && node != newLink->getStartNode());
    });

    if (itNode != nodes.end())
    {
        qDebug() << "Valid Link";
        newLink->addEndingNode(*itNode);
        newLink->getStartNode()->addLink(newLink);

        connect(newLink, &Link::removeMe, this, &ToolBoxScene::removeLink);
    }
    else
    {
        qDebug() <<"Not a valid Link";
        removeItem(newLink);
        links.erase(std::find(links.begin(), links.end(), newLink));

        if (newLink != nullptr) delete newLink;
    }
}

void ToolBoxScene::removeNode()
{
    Node *node = static_cast<Node*>(sender());

    // erase every link which is connected to the node
    links.erase(std::remove_if(links.begin(), links.end(), [&](Link *link)
    {
        if ((link->getStartNode() == node) || (link->getEndNode() == node))
        {
            link->getStartNode()->removeLink(link);
            delete link;
            link = nullptr;
            return true;
        }
        return false;
    }), links.end());

    nodes.erase(std::find(nodes.cbegin(), nodes.cend(), node));
    delete node;
}

void ToolBoxScene::removeLink()
{
    Link *link = static_cast<Link*>(sender());

    link->getStartNode()->removeLink(link);

    delete link;
}

void ToolBoxScene::nodeIsCopied()
{
    copiedNode = static_cast<Node*>(sender());

    qDebug() << copiedNode->objectName();
}

void ToolBoxScene::nodeSelected()
{
    Node *selected = static_cast<Node*>(sender());

    emit displayStep(selected);
}

void ToolBoxScene::checkNaming(QString name)
{
    QString tmpName = name;
    Node *node = static_cast<Node*>(sender());

    int cnt = 1;

    while (std::find_if(nodes.begin(), nodes.end(), [=](Node *tmp) { return (tmp->toPlainText() == tmpName) && (tmp != node);}) != nodes.end())
    {
        tmpName = name + '_' + QString::number(cnt);
        cnt++;
    }

    node->setPlainText(tmpName);
}

const QVector<Link *> &ToolBoxScene::getLinks() const
{
    return links;
}

const QVector<Node *> &ToolBoxScene::getNodes() const
{
    return nodes;
}

Node *ToolBoxScene::getNode(QString tag)
{
    auto foundIt = std::find_if(nodes.begin(), nodes.end(), [&](Node * node){return node->toPlainText() == tag;});

    if (foundIt != nodes.end())
    {
        return *foundIt;
    }
    else
    {
        return nullptr;
    }
}

