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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
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
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *coordinates;
    QLabel *coordinates_3;
    QSpinBox *thetaRobot;
    QCheckBox *checkBox;
    QToolBox *toolBox;
    QWidget *Actions;
    QWidget *ActionsRobot;
    QWidget *MetaActions;
    QGraphicsView *playgroundScene;
    QGraphicsView *graphicsView;
    QMenuBar *menubar;
    QMenu *menuFiles;
    QMenu *menuexport;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1000, 600);
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
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        coordinates = new QLabel(centralwidget);
        coordinates->setObjectName("coordinates");
        QFont font;
        font.setPointSize(14);
        coordinates->setFont(font);

        gridLayout->addWidget(coordinates, 0, 0, 1, 1);

        coordinates_3 = new QLabel(centralwidget);
        coordinates_3->setObjectName("coordinates_3");
        coordinates_3->setFont(font);

        gridLayout->addWidget(coordinates_3, 0, 1, 1, 1);

        thetaRobot = new QSpinBox(centralwidget);
        thetaRobot->setObjectName("thetaRobot");
        thetaRobot->setMinimum(-3600);
        thetaRobot->setMaximum(3600);

        gridLayout->addWidget(thetaRobot, 0, 2, 1, 1);

        checkBox = new QCheckBox(centralwidget);
        checkBox->setObjectName("checkBox");

        gridLayout->addWidget(checkBox, 0, 3, 1, 1);

        toolBox = new QToolBox(centralwidget);
        toolBox->setObjectName("toolBox");
        toolBox->setLineWidth(3);
        Actions = new QWidget();
        Actions->setObjectName("Actions");
        Actions->setGeometry(QRect(0, 0, 598, 60));
        toolBox->addItem(Actions, QString::fromUtf8("Common"));
        ActionsRobot = new QWidget();
        ActionsRobot->setObjectName("ActionsRobot");
        ActionsRobot->setGeometry(QRect(0, 0, 598, 60));
        toolBox->addItem(ActionsRobot, QString::fromUtf8("Robot Actions"));
        MetaActions = new QWidget();
        MetaActions->setObjectName("MetaActions");
        MetaActions->setGeometry(QRect(0, 0, 598, 60));
        toolBox->addItem(MetaActions, QString::fromUtf8("Meta Actions"));

        gridLayout->addWidget(toolBox, 1, 0, 1, 4);


        gridLayout_2->addLayout(gridLayout, 1, 0, 1, 1);

        playgroundScene = new QGraphicsView(centralwidget);
        playgroundScene->setObjectName("playgroundScene");
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(playgroundScene->sizePolicy().hasHeightForWidth());
        playgroundScene->setSizePolicy(sizePolicy);
        playgroundScene->setMinimumSize(QSize(600, 400));
        playgroundScene->setMaximumSize(QSize(600, 400));
        playgroundScene->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        playgroundScene->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        gridLayout_2->addWidget(playgroundScene, 0, 0, 1, 1);

        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setMouseTracking(false);
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        graphicsView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        graphicsView->setAlignment(Qt::AlignCenter);
        graphicsView->setDragMode(QGraphicsView::NoDrag);
        graphicsView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
        graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

        gridLayout_2->addWidget(graphicsView, 0, 1, 2, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1000, 20));
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
        coordinates->setText(QCoreApplication::translate("MainWindow", "X : 0  Y : 0", nullptr));
        coordinates_3->setText(QCoreApplication::translate("MainWindow", "Theta :", nullptr));
        checkBox->setText(QCoreApplication::translate("MainWindow", "PotiBot", nullptr));
        toolBox->setItemText(toolBox->indexOf(Actions), QCoreApplication::translate("MainWindow", "Common", nullptr));
        toolBox->setItemText(toolBox->indexOf(ActionsRobot), QCoreApplication::translate("MainWindow", "Robot Actions", nullptr));
        toolBox->setItemText(toolBox->indexOf(MetaActions), QCoreApplication::translate("MainWindow", "Meta Actions", nullptr));
        menuFiles->setTitle(QCoreApplication::translate("MainWindow", "Files", nullptr));
        menuexport->setTitle(QCoreApplication::translate("MainWindow", "export", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
