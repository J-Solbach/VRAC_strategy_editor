#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "toolboxscene.h"
#include <QJsonObject>
#include <QJsonDocument>
#include "playground.h"

#define SCALING 0.2
#define NB_COLONNES 3

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QToolButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

    void setupToolbar();

    void setupCommonActions();

    void setupGrobotActions();

    //void setupPotibotActions();

    void setupMetaActions();

    int countLink(Node *selectedNode);

    void organize_MetaAction(QString fileName,ToolBoxScene *scene);

public slots:

    void actionButtonClicked();

    void groBotActionButtonClicked();

    void potiBotActionButtonClicked();

    void metaActionButtonClicked();

    void updatePos(position pos);

    void displayStep(Node *displayedNode);

    void simulateStep(Node *simulateNode);

    void metaStep(Node *selectedNode, ToolBoxScene *scene);

    void metaSimulateStep(Node *selectedNode, ToolBoxScene *scene);

    void Load_MetaAction(QString fileName);

private slots:
    void on_actionsave_triggered();

    void on_actionLoad_MetaAction_triggered();

    void on_thetaRobot_valueChanged(int arg1);

    void on_actionNew_MetaAction_triggered(bool checked);

    void on_checkBox_stateChanged(int arg1);

    void on_actionOrganize_triggered();

private:
    Ui::MainWindow *ui;

    Playground *playground;

    ToolBoxScene stratBuilder;

    QJsonObject commonActions;

    QJsonObject groBotActions;

    QJsonObject potiBotActions;

    QList<QToolButton*> metaButtons;

    bool first = true;
};
#endif // MAINWINDOW_H
