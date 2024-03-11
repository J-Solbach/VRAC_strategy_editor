#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QGraphicsRectItem>
#include <QGridLayout>
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
#include <playground.h>
#include <QFileInfo>
#include "jsonhelperfunctions.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    playground = new Playground();
    ui->playgroundScene->setScene(playground);
    ui->playgroundScene->scale(SCALING,SCALING);

    ui->graphicsView->setScene(&stratBuilder);
    ui->graphicsView->viewport()->setCursor(Qt::ArrowCursor);
    stratBuilder.setItemIndexMethod(QGraphicsScene::NoIndex);

    setupToolbar();

    connect(playground, &Playground::displayNewPos, this, &MainWindow::updatePos);
    connect(&stratBuilder, &ToolBoxScene::displayStep, this, &MainWindow::displayStep);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupToolbar()
{
    setupCommonActions();
    setupGrobotActions();
    //setupPotibotActions();
    setupMetaActions();
}

void MainWindow::setupCommonActions()
{
    QGridLayout *grid = new QGridLayout();
    grid->setHorizontalSpacing(10);
    grid->setVerticalSpacing(10);

    QFile File;

    File.setFileName(":/config/commonActions.json");
    File.open(QIODevice::ReadOnly | QIODevice::Text);
    commonActions = QJsonDocument::fromJson(QString(File.readAll()).toUtf8()).object();

    int counter = 0;
    for (auto key : commonActions.keys())
    {
        QToolButton *actionButton = new QToolButton();
        actionButton->setText(key);
        actionButton->setMaximumWidth(200);
        actionButton->setStyleSheet("font-size:14px;");
        actionButton->setMinimumHeight(50);
        actionButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        connect(actionButton, &QToolButton::clicked, this, &MainWindow::actionButtonClicked);

        grid->addWidget(actionButton, counter/NB_COLONNES, counter%NB_COLONNES);
        counter++;
    }

    ui->Actions->setLayout(grid);
}

void MainWindow::setupGrobotActions()
{
    QGridLayout *grid = new QGridLayout();
    grid->setHorizontalSpacing(10);
    grid->setVerticalSpacing(10);

    QFile File;

    File.setFileName(":/config/groBotActions.json");
    File.open(QIODevice::ReadOnly | QIODevice::Text);
    groBotActions = QJsonDocument::fromJson(QString(File.readAll()).toUtf8()).object();

    int counter = 0;
    for (auto key : groBotActions.keys())
    {
        QToolButton *actionButton = new QToolButton();
        actionButton->setText(key);
        actionButton->setStyleSheet("font-size:14px;");
        actionButton->setMinimumHeight(50);

        actionButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        connect(actionButton, &QToolButton::clicked, this, &MainWindow::groBotActionButtonClicked);

        grid->addWidget(actionButton, counter/NB_COLONNES, counter%NB_COLONNES);
        counter++;
    }

    ui->ActionsRobot->setLayout(grid);
}

/*void MainWindow::setupPotibotActions()
{
    QGridLayout *grid = new QGridLayout();
    grid->setHorizontalSpacing(10);
    grid->setVerticalSpacing(10);

    QFile File;

    File.setFileName(QDir::homePath() + "/Documents/Workspace/VRAC/StrategyViewer/config/potiBotActions.json");
    File.open(QIODevice::ReadOnly | QIODevice::Text);
    potiBotActions = QJsonDocument::fromJson(QString(File.readAll()).toUtf8()).object();

    int counter = 0;
    for (auto &key : potiBotActions.keys())
    {
        QToolButton *actionButton = new QToolButton();
        actionButton->setText(key);
        actionButton->setStyleSheet("font-size:14px;");
        actionButton->setMinimumHeight(50);

        actionButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        connect(actionButton, &QToolButton::clicked, this, &MainWindow::potiBotActionButtonClicked);

        grid->addWidget(actionButton, counter/NB_COLONNES, counter%NB_COLONNES);
        counter++;
    }

    ui->ActionsPotibot->setLayout(grid);
}*/

void MainWindow::setupMetaActions()
{
    if (ui->MetaActions->layout())
    {
        for (auto &button : metaButtons)
        {
            ui->MetaActions->layout()->removeWidget(button);
            delete button;
        }
        metaButtons.clear();
        delete ui->MetaActions->layout();
    }

    QGridLayout *grid = new QGridLayout();
    grid->setHorizontalSpacing(10);
    grid->setVerticalSpacing(10);

    QDir directory;
    directory = QDir(":/config/metaActions/");

    QStringList metaActions = directory.entryList(QStringList() << "*.json" << "*.JSON",QDir::Files);

    int counter = 0;
    for(auto &metaActionFile : metaActions) {
        QToolButton *actionButton = new QToolButton();
        actionButton->setText(QFileInfo(metaActionFile).baseName());
        actionButton->setStyleSheet("font-size:14px;");
        actionButton->setMinimumHeight(50);
        actionButton->setMaximumHeight(75);
        actionButton->setMaximumWidth(300);
        actionButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        metaButtons << actionButton;

        connect(actionButton, &QToolButton::clicked, this, &MainWindow::metaActionButtonClicked );

        grid->addWidget(actionButton, counter/NB_COLONNES, counter%NB_COLONNES);
        counter++;
    }

    ui->MetaActions->setLayout(grid);
}

void MainWindow::actionButtonClicked()
{
    QToolButton *actionButton = static_cast<QToolButton*>(sender());

    QJsonValue action(commonActions[actionButton->text()].toObject());

    for (auto &k : action["parameters"].toObject().keys())
    {
        if (k == "y")
        {
            modifyJsonValue(action, "parameters."+ k, playground->getRobot().pos().coord.x() );
        }
        else if (k == "x")
        {
            modifyJsonValue(action, "parameters."+ k, playground->getRobot().pos().coord.y() );
        }
        else if (k == "theta")
        {
            modifyJsonValue(action, "parameters."+ k, playground->getRobot().pos().theta);
        }
    }

    Node *actionNode = new Node(actionButton->text(), action.toObject());
    actionNode->setPos(100,100);

    stratBuilder.addNode(actionNode);
    stratBuilder.update();

    actionNode->setupName();
}

void MainWindow::groBotActionButtonClicked()
{
    QToolButton *actionButton = static_cast<QToolButton*>(sender());
    Node *actionNode = new Node(actionButton->text(), groBotActions[actionButton->text()].toObject());

    stratBuilder.addNode(actionNode);
    stratBuilder.update();

    actionNode->setupName();
}

void MainWindow::potiBotActionButtonClicked()
{
    QToolButton *actionButton = static_cast<QToolButton*>(sender());
    Node *actionNode = new Node(actionButton->text(), potiBotActions[actionButton->text()].toObject());

    stratBuilder.addNode(actionNode);
    stratBuilder.update();

    actionNode->setupName();
}

void MainWindow::metaActionButtonClicked()
{
    QToolButton *actionButton = static_cast<QToolButton*>(sender());

    QJsonObject metaActionObject {{"tag",QJsonValue::fromVariant(actionButton->text())}, {"file", QJsonValue::fromVariant(actionButton->text())}};

    Node *metaActionNode = new Node (actionButton->text(), metaActionObject);
    stratBuilder.addNode(metaActionNode);
    stratBuilder.update();

    metaActionNode->setupName();
}

void MainWindow::updatePos(position pos)
{
    QString text;
    text = "X : " + QString::number(pos.coord.y() ) + " Y : " + QString::number(pos.coord.x() );
    ui->coordinates->setText(text);
    ui->thetaRobot->setValue(pos.theta);
}

int MainWindow::countLink(Node *selectedNode)
{
    int nb_link=0;
    auto links = stratBuilder.getLinks();

    std::for_each(links.begin(), links.end(),[&](Link *link)
    {
        if(link->getEndNode() == selectedNode)nb_link++;
    });
    return nb_link;
}

void MainWindow::simulateStep(Node *simulateNode)
{
    Node * nextNode = nullptr;
    auto links = stratBuilder.getLinks();

    std::for_each(links.begin(), links.end(),[&](Link *link)
    {
        if(link->getStartNode() == simulateNode)
        {
            nextNode = link->getEndNode();
            if(nextNode!=nullptr)
            {
                nextNode->setPreviousStartNode(simulateNode->toPlainText());
                qDebug()<<nextNode->toPlainText()<<"<-"<<nextNode->getPreviousStartNode();
            }
            simulateStep(nextNode);
        }
    });
}

void MainWindow::displayStep(Node *selectedNode)
{
    playground->resetItems();

    if(first)
    {
        qDebug()<<selectedNode->toPlainText();
        simulateStep(selectedNode);
        first=false;
    }

    Node * previousNode = nullptr;

    auto links = stratBuilder.getLinks();

    auto itLink = std::find_if(links.begin(), links.end(), [&](Link *link)
    {
        if( link != nullptr)
        {
            if(link->getEndNode() == selectedNode)
            {
                return(link->getStartNode()->toPlainText()==selectedNode->getPreviousStartNode());
            }
        }
        return false;
    });

    if (itLink != links.end())
    {
        previousNode = (*itLink)->getStartNode();
        qDebug()<< " Previous :" << previousNode->toPlainText()<<" connect: "<<countLink(previousNode);
    }

    if (previousNode != nullptr)
    {
        // rewind and display all previous nodes
        displayStep(previousNode);

        if (previousNode->getAction()["action"].toString() != "Bezier"
            && previousNode->getAction()["action"].toString() != "Rotate"
            && previousNode->getAction()["action"].toString() != "Line"
            && previousNode->getAction()["displayed"].toBool())
        {
            playground->setCurrentDisplayedNode(previousNode);
            qDebug() << "Previous :" << previousNode->toPlainText();
        }
    }

    if (selectedNode->getAction()["displayed"].toBool())
    {
        playground->setCurrentDisplayedNode(selectedNode);
    }

    first=true;
}


void MainWindow::on_actionsave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName();
    QFile f(fileName);

    f.open(QIODevice::WriteOnly);

    QJsonDocument testDoc;
    QJsonObject rootTest = testDoc.object();

    rootTest.insert("name", QFileInfo(fileName).baseName());

    QJsonArray arr;

    for (auto it : stratBuilder.getNodes())
    {
        arr.append(it->getAction());
    }

    rootTest.insert("actions", arr);

    testDoc.setObject(rootTest);
    f.write(testDoc.toJson());
    f.close();

    setupMetaActions();
}


void MainWindow::on_actionLoad_MetaAction_triggered()
{
    QString fileName = QFileDialog::getOpenFileName();

    if (fileName.isEmpty()) return;

    stratBuilder.clearScene();

    QFile f(fileName);

    f.open(QIODevice::ReadOnly);

    QJsonDocument doc = QJsonDocument::fromJson(QString(f.readAll()).toUtf8());
    QJsonObject rootTest = doc.object();

    QJsonArray actions = rootTest["actions"].toArray();

    auto action = actions.first().toObject();

    for (auto actionRef : actions)
    {
        auto action = actionRef.toObject();
        Node *testNode = new Node(action["tag"].toString(), action);
        stratBuilder.addNode(testNode);
        stratBuilder.update();
    }

    //setupLinks
    for (auto actionRef : actions)
    {
        auto action = actionRef.toObject();

        Node *startNode = stratBuilder.getNode(action["tag"].toString());

        if (startNode== nullptr) continue;

        QJsonArray transitions = action["transitions"].toArray();

        for (auto transitionRef : transitions)
        {
            auto transition = transitionRef.toObject();
            Node *endNode = stratBuilder.getNode(transition["destination"].toString());

            if (endNode != nullptr)
            {
                Link *newLink = new Link();

                newLink->addStartingNode(startNode);
                startNode->addLink(newLink);
                newLink->addEndingNode(endNode);
                newLink->getTransition().setText(transition["type"].toString());

                stratBuilder.addLink(newLink);
            }
        }
    }

    Node * currentNode = stratBuilder.getNodes().first();
    QPointF currentPos(stratBuilder.sceneRect().center().x(), -40);

   stratBuilder.organizeScene(currentNode, currentPos);

   stratBuilder.update();
}


void MainWindow::on_thetaRobot_valueChanged(int arg1)
{
   playground->newTheta((double)arg1);
}


void MainWindow::on_actionNew_MetaAction_triggered(bool checked)
{
    stratBuilder.clearScene();
    setupMetaActions();
}


void MainWindow::on_checkBox_stateChanged(int)
{
    qDebug() << "check box changed";
    setupMetaActions();

    if (ui->checkBox->isChecked())
    {
        playground->getRobot().setNewPixmap(":Images/potibot.png");
    }
    else
    {
        playground->getRobot().setNewPixmap(":Images/grobot.png");
    }
}


void MainWindow::on_actionOrganize_triggered()
{
    auto nodes = stratBuilder.getNodes();
    if (nodes.isEmpty()) return;

    Node * currentNode = nodes.first();

    QPointF currentPos(stratBuilder.sceneRect().center().x(), 0);

    stratBuilder.organizeScene(currentNode, currentPos);

    stratBuilder.update();
}

