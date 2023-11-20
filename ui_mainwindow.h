/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew_MetaAction;
    QAction *actionLoad_MetaAction;
    QAction *actionsave;
    QAction *actionOrganize;
    QWidget *centralwidget;
    QGraphicsView *graphicsView;
    QToolBox *toolBox;
    QWidget *Actions;
    QWidget *ActionsRobot;
    QWidget *MetaActions;
    QGraphicsView *playgroundScene;
    QLabel *coordinates;
    QSpinBox *thetaRobot;
    QLabel *coordinates_3;
    QCheckBox *checkBox;
    QMenuBar *menubar;
    QMenu *menuFiles;
    QMenu *menuexport;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1280, 950);
        actionNew_MetaAction = new QAction(MainWindow);
        actionNew_MetaAction->setObjectName("actionNew_MetaAction");
        actionLoad_MetaAction = new QAction(MainWindow);
        actionLoad_MetaAction->setObjectName("actionLoad_MetaAction");
        actionsave = new QAction(MainWindow);
        actionsave->setObjectName("actionsave");
        actionOrganize = new QAction(MainWindow);
        actionOrganize->setObjectName("actionOrganize");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setGeometry(QRect(620, 10, 660, 900));
        graphicsView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        graphicsView->setDragMode(QGraphicsView::NoDrag);
        graphicsView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
        graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
        toolBox = new QToolBox(centralwidget);
        toolBox->setObjectName("toolBox");
        toolBox->setGeometry(QRect(10, 450, 600, 450));
        toolBox->setLineWidth(3);
        Actions = new QWidget();
        Actions->setObjectName("Actions");
        Actions->setGeometry(QRect(0, 0, 600, 375));
        toolBox->addItem(Actions, QString::fromUtf8("Common"));
        ActionsRobot = new QWidget();
        ActionsRobot->setObjectName("ActionsRobot");
        ActionsRobot->setGeometry(QRect(0, 0, 600, 375));
        toolBox->addItem(ActionsRobot, QString::fromUtf8("Robot Actions"));
        MetaActions = new QWidget();
        MetaActions->setObjectName("MetaActions");
        MetaActions->setGeometry(QRect(0, 0, 600, 375));
        toolBox->addItem(MetaActions, QString::fromUtf8("Meta Actions"));
        playgroundScene = new QGraphicsView(centralwidget);
        playgroundScene->setObjectName("playgroundScene");
        playgroundScene->setGeometry(QRect(10, 10, 600, 400));
        playgroundScene->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        playgroundScene->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        coordinates = new QLabel(centralwidget);
        coordinates->setObjectName("coordinates");
        coordinates->setGeometry(QRect(10, 420, 291, 20));
        QFont font;
        font.setPointSize(14);
        coordinates->setFont(font);
        thetaRobot = new QSpinBox(centralwidget);
        thetaRobot->setObjectName("thetaRobot");
        thetaRobot->setGeometry(QRect(190, 420, 111, 24));
        thetaRobot->setMinimum(-3600);
        thetaRobot->setMaximum(3600);
        coordinates_3 = new QLabel(centralwidget);
        coordinates_3->setObjectName("coordinates_3");
        coordinates_3->setGeometry(QRect(130, 420, 51, 20));
        coordinates_3->setFont(font);
        checkBox = new QCheckBox(centralwidget);
        checkBox->setObjectName("checkBox");
        checkBox->setGeometry(QRect(320, 420, 86, 21));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1280, 22));
        menuFiles = new QMenu(menubar);
        menuFiles->setObjectName("menuFiles");
        menuexport = new QMenu(menubar);
        menuexport->setObjectName("menuexport");
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menuFiles->menuAction());
        menubar->addAction(menuexport->menuAction());
        menuFiles->addAction(actionNew_MetaAction);
        menuFiles->addAction(actionLoad_MetaAction);
        menuFiles->addAction(actionOrganize);
        menuexport->addAction(actionsave);

        retranslateUi(MainWindow);

        toolBox->setCurrentIndex(0);
        toolBox->layout()->setSpacing(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionNew_MetaAction->setText(QCoreApplication::translate("MainWindow", "New File", nullptr));
        actionLoad_MetaAction->setText(QCoreApplication::translate("MainWindow", "Load File", nullptr));
        actionsave->setText(QCoreApplication::translate("MainWindow", "save", nullptr));
        actionOrganize->setText(QCoreApplication::translate("MainWindow", "Organize", nullptr));
        toolBox->setItemText(toolBox->indexOf(Actions), QCoreApplication::translate("MainWindow", "Common", nullptr));
        toolBox->setItemText(toolBox->indexOf(ActionsRobot), QCoreApplication::translate("MainWindow", "Robot Actions", nullptr));
        toolBox->setItemText(toolBox->indexOf(MetaActions), QCoreApplication::translate("MainWindow", "Meta Actions", nullptr));
        coordinates->setText(QCoreApplication::translate("MainWindow", "X : 0  Y : 0", nullptr));
        coordinates_3->setText(QCoreApplication::translate("MainWindow", "Theta :", nullptr));
        checkBox->setText(QCoreApplication::translate("MainWindow", "PotiBot", nullptr));
        menuFiles->setTitle(QCoreApplication::translate("MainWindow", "Files", nullptr));
        menuexport->setTitle(QCoreApplication::translate("MainWindow", "export", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
