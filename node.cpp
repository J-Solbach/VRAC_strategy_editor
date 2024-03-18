#include "node.h"
#include <QPainter>
#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QDialog>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QJsonArray>
#include <QVariantList>
#include <QStringList>
#include "jsonhelperfunctions.h"
#include "mainwindow.h"
#include <QGraphicsView>

Node::Node(QString tag, QJsonObject json, QGraphicsItem *parent) : QGraphicsTextItem(tag, parent) , action(json), m_tag(tag)
{
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);

    modifyJsonValue(action, "tag.", m_tag);

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    setTextInteractionFlags(Qt::TextSelectableByMouse);

    QFont font;
    font.setPixelSize(18);
    setFont(font);

    if(isMetaAction())
    {
        MainWindow main;

        stratBuilder=new ToolBoxScene;

        main.organize_MetaAction(getfileName(),stratBuilder);
    }
}


QRectF Node::boundingRect() const
{
    QRectF rect = QGraphicsTextItem::boundingRect();
    rect.setX(rect.x() - 5);
    rect.setY(rect.y() - 5);
    rect.setWidth(rect.width() + 10);
    rect.setHeight(rect.height() + 10);

    return rect;
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(Qt::magenta, (isHovered) ? 3 : 1));
    painter->setBrush(QColor("#333"));

    if (isSelected()) painter->setBrush(QColor("#222"));

    QRectF rect = boundingRect();
    rect.setX(rect.x() + 5);
    rect.setY(rect.y() + 5);
    rect.setWidth(rect.width() - 10);
    rect.setHeight(rect.height() - 10);
    painter->drawRoundedRect(rect , 5, 5);

    painter->setBrush(Qt::magenta);

    outPort = QRectF(rect.x()+ rect.width()/2 - 3, rect.y() + rect.height() - 3, 6, 6);
    inPort = QRectF(rect.x()+ rect.width()/2 - 3, rect.y() - 3, 6, 6);
    painter->drawEllipse(outPort);
    painter->drawEllipse(inPort);

    QGraphicsTextItem::paint(painter,option,widget);
}

void Node::addLink(Link *link)
{
    if (links.contains(link))
        return;

    links.append(link);
}

void Node::removeLink(Link *link)
{
    links.erase(std::remove(links.begin(), links.end(), link), links.cend());
}

void Node::setPreviousStartNode(QString previousStartNode)
{
    m_previousStartNode=previousStartNode;
}

bool Node::isMetaAction()
{
    return(!action["file"].isUndefined());
}

void Node::setupName()
{
    if (action["file"].isUndefined())
    {
        QString name = action["action"].toString();
        for (auto &k : action["parameters"].toObject().keys())
        {
            if (action["parameters"].toObject()[k].isObject())
            {
                name += "_" + action["parameters"].toObject()[k].toObject()["type"].toString();
            }
        }
         nameChanged(name);
    }
    else
    {
        nameChanged(action["file"].toString());
    }
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (outPort.contains(event->pos()))
        {
            emit outPortClicked();
            ungrabMouse();
        }
        else
        {
            startPos = event->pos();
        }
    }

    QGraphicsTextItem::mousePressEvent(event);

    emit selected();
}

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QDialog dialog;
    dialog.setWindowTitle(toPlainText());

    QVBoxLayout layout;

    if(!isMetaAction())
    {
        if (action["parameters"].toObject().keys().isEmpty()) return;

        for (auto &k : action["parameters"].toObject().keys())
        {
            QHBoxLayout *hLayout = new QHBoxLayout();
            if (action["parameters"].toObject()[k].isBool())
            {
                QCheckBox *box = new QCheckBox(k);
                box->setLayoutDirection(Qt::LayoutDirection::LayoutDirectionAuto);
                box->setChecked(action["parameters"].toObject()[k].toBool());
                hLayout->addWidget(box);

                connect(box, &QCheckBox::clicked, this, [&](bool checked)
                        {
                            QCheckBox *box = static_cast<QCheckBox*>(sender());
                            modifyJsonValue(action, "parameters."+ box->text(), checked);
                            emit selected();
                        });
            }
            else if (action["parameters"].toObject()[k].isDouble())
            {
                QSpinBox *spinBox = new QSpinBox();
                spinBox->setObjectName(k);
                spinBox->setMinimum(-100000);
                spinBox->setMaximum(100000);
                spinBox->setValue(action["parameters"].toObject()[k].toDouble());

                hLayout->addWidget(spinBox);
                hLayout->addWidget(new QLabel(k));

                connect(spinBox, &QSpinBox::valueChanged, this, [&](int newValue)
                        {
                            QSpinBox *spinBoxSdr = static_cast<QSpinBox*>(sender());
                            modifyJsonValue(action, "parameters."+spinBoxSdr->objectName(), newValue);
                            emit selected();
                        });
            }
            else if (action["parameters"].toObject()[k].isObject())
            {
                QComboBox *comboBox = new QComboBox();
                comboBox->setObjectName(k);
                QStringList list;

                for (const auto &str : action["parameters"].toObject()[k].toObject()["values"].toArray())
                {
                    list.append(str.toString());
                }
                comboBox->addItems(list);
                comboBox->setCurrentIndex(comboBox->findText(action["parameters"].toObject()[comboBox->objectName()].toObject()["type"].toString()));

                hLayout->addWidget(comboBox);

                connect(comboBox, &QComboBox::currentTextChanged, this, [&](QString newText)
                        {
                            QComboBox *comboBox = static_cast<QComboBox*>(sender());
                            modifyJsonValue(action, "parameters."+comboBox->objectName()+".type", newText);
                            emit selected();
                        });
            }
            layout.addLayout(hLayout);
        }
    }
    else
    {
        MainWindow main;
        QGraphicsView *graphicsview= new QGraphicsView;
        graphicsview->setScene(stratBuilder);

        Node * currentNode = stratBuilder->getNodes().first();
        QPointF currentPos(stratBuilder->sceneRect().center().x(), -40);

        stratBuilder->organizeScene(currentNode, currentPos);

        connect(stratBuilder, &ToolBoxScene::displayStep, this,[&](Node *selectedNode)
        {
            main.metaStep(selectedNode,stratBuilder);
        });

        layout.addWidget(graphicsview);
    }

    dialog.setLayout(&layout);
    dialog.updateGeometry();
    dialog.exec();

    setupName();
}


void Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (startPos != event->pos())
    {
        QPointF delta = event->pos() - startPos;

        if (isMoving || delta.manhattanLength() >= QApplication::startDragDistance() )
        {
            setPos(pos() + delta);
            emit moved(delta);
            isMoving = true;
        }
    }

    QGraphicsTextItem::mouseMoveEvent(event);

}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mouseReleaseEvent(event);

    isMoving = false;
}


void Node::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    update();
    QGraphicsTextItem::hoverEnterEvent(event);
}

void Node::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::ArrowCursor);

    if (outPort.contains(event->pos()) || inPort.contains(event->pos()))
    {
        setCursor(Qt::CrossCursor);
    }

    isHovered = true;
    update();
    QGraphicsTextItem::hoverEnterEvent(event);
}

void Node::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    isHovered = false;
    update();
    QGraphicsTextItem::hoverEnterEvent(event);
}

void Node::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;

    QAction *editAction;

    if(isMetaAction())
    {
        editAction = menu.addAction("Edit");
    }

    QAction *removeAction = menu.addAction("Remove");

    QAction *selectedAction = menu.exec(event->screenPos());

    if (selectedAction == removeAction)
    {
        emit removeMe();
    }
    else if(selectedAction == editAction)
    {
        emit MetaActionSelected(getfileName());
    }
    // ...
}

const QVector<Link *> &Node::getLinks() const
{
    return links;
}

QJsonValue Node::getAction()
{
    QJsonArray arr;

    for (auto link : links)
    {
        if (link == nullptr) continue;

        QJsonObject object{
            {"type", link->getTransition().text()},
            {"destination", link->getEndNode()->toPlainText()}
        };

        arr.append(object);
    }

    modifyJsonValue(action, "transitions.", arr);
    modifyJsonValue(action, "tag.", toPlainText());

    return action;
}


