#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "toolboxscene.h"
#include <QJsonObject>
#include <QJsonDocument>
#include "playground.h"

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

public slots:

    void actionButtonClicked();

    void groBotActionButtonClicked();

    void potiBotActionButtonClicked();

    void metaActionButtonClicked();

    void updatePos(position pos);

    void displayStep(Node *displayedNode);


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
};
#endif // MAINWINDOW_H
