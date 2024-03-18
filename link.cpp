#include "link.h"
#include "node.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QEvent>
#include <QCursor>
#include <QDialog>
#include <QLayout>
#include <QComboBox>
#include <QMenu>

Link::Link() : QObject(), QGraphicsLineItem()
{
    setFlag(QGraphicsItem::ItemIsFocusable);

    QFont font;
    font.setPixelSize(14);
    transition.setFont(font);
    transition.setText("NoEvent");
    transition.setFlag(QGraphicsTextItem::ItemIsMovable);
}


void Link::addStartingNode(Node *node)
{
    grabMouse();
    startNode = node;
    mousePos = mapFromItem(startNode, startNode->getOutPort());
}

QRectF Link::boundingRect() const
{
    QRectF rect = QGraphicsLineItem::boundingRect();
    rect.setX(rect.x() - transition.boundingRect().width());
    rect.setY(rect.y() - transition.boundingRect().height());
    rect.setWidth(rect.width() + transition.boundingRect().width());
    rect.setHeight(rect.height() + transition.boundingRect().height());

    return rect;
}

void Link::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QLineF line;

    if (endNode == nullptr)
    {
        line = QLineF(mapFromItem(startNode, startNode->getOutPort()), mousePos);
    }
    else
    {
        line = QLineF(mapFromItem(startNode, startNode->getOutPort()), mapFromItem(endNode, endNode->getInPort()));
    }

    setLine(line);
    setPen(m_pen);
    QGraphicsLineItem::paint(painter, option, widget);

    painter->setPen(Qt::white);
    painter->drawText(boundingRect(), Qt::AlignCenter, transition.text());
}

void Link::changePen(QPen pen)
{
    m_pen=pen;
}

void Link::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    mousePos = event->pos();
    update();
    QGraphicsLineItem::mouseMoveEvent(event);
}

void Link::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsLineItem::mouseReleaseEvent(event);
    mousePos = event->pos();
    ungrabMouse();
    update();
    emit released();
}

void Link::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QDialog dialog;
    dialog.setWindowTitle("Transition");
    QHBoxLayout layout;
    QComboBox transiBox;

   transiBox.addItem("NoEvent");
   transiBox.addItem("Failed");
   transiBox.addItem("Timeout");
   transiBox.addItem("EmergencyBrake");
   transiBox.addItem("Resume");
   transiBox.addItem("NewPathFound");
   transiBox.addItem("AckMoveHoming");
   transiBox.addItem("AckMoveLine");
   transiBox.addItem("AckMoveRotate");
   transiBox.addItem("AckMoveBezier");
   transiBox.addItem("AckMoveStopped");
   transiBox.addItem("AckMoveXYT");
   transiBox.addItem("AckEndMove");
   transiBox.addItem("AckServo");
   transiBox.addItem("AckStepper");
   transiBox.addItem("AckDCMotor");
   transiBox.addItem("TorActive");
   transiBox.addItem("TorInactive");
   transiBox.addItem("CountDecreased");
   transiBox.addItem("CountFinished");
   transiBox.addItem("IsAtHome");
   transiBox.addItem("YouCanGo");
   transiBox.addItem("Finished");
   transiBox.addItem("Jack");
   transiBox.addItem("Blocked");
   transiBox.addItem("EnnemyDetected");
   transiBox.addItem("MineDetected");

   transiBox.addItem("opponentsStartOn_TopRight");
   transiBox.addItem("opponentsStartOn_Left");
   transiBox.addItem("opponentsStartOn_Right");
   transiBox.addItem("opponentsStartOn_BottomLeft");
   transiBox.addItem("opponentsStartOn_BottomRight");

   transiBox.addItem("opponentsStartOn_TopRight_Left");
   transiBox.addItem("opponentsStartOn_TopRight_Right");
   transiBox.addItem("opponentsStartOn_TopRight_BottomLeft");
   transiBox.addItem("opponentsStartOn_TopRight_BottomRight");

   transiBox.addItem("opponentsStartOn_Left_Right");
   transiBox.addItem("opponentsStartOn_Left_BottomLeft");
   transiBox.addItem("opponentsStartOn_Left_BottomRight");

   transiBox.addItem("opponentsStartOn_Right_BottomLeft");
   transiBox.addItem("opponentsStartOn_Right_BottomRight");

   transiBox.addItem("opponentsStartOn_BottomLeft_BottomRight");

   transiBox.setCurrentText(transition.text());

    connect(&transiBox, &QComboBox::currentTextChanged, this, [&](QString newText)
    {
        transition.setText(newText);
        dialog.accept();
    });

    layout.addWidget(&transiBox);
    dialog.setLayout(&layout);
    dialog.exec();
}


void Link::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::ArrowCursor);
    QGraphicsLineItem::hoverMoveEvent(event);
}


Node *Link::getStartNode() const
{
    return startNode;
}

Node *Link::getEndNode() const
{
    return endNode;
}


bool Link::sceneEvent(QEvent *event)
{
    if (event->type() == QEvent::GrabMouse)
    {
        event->accept();
        return true;
    }

    return QGraphicsItem::sceneEvent(event);
}


void Link::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    QAction *removeAction = menu.addAction("Remove");
    QAction *selectedAction = menu.exec(event->screenPos());

    if (selectedAction == removeAction)
    {
        emit removeMe();
    }
    // ...
}
