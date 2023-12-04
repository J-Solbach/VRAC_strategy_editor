QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    jsonhelperfunctions.cpp \
    link.cpp \
    main.cpp \
    mainwindow.cpp \
    node.cpp \
    playground.cpp \
    pot.cpp \
    robot.cpp \
    toolboxscene.cpp

HEADERS += \
    ../LIBS/common/shared_types.hpp \
    jsonhelperfunctions.h \
    link.h \
    mainwindow.h \
    node.h \
    playground.h \
    pot.h \
    robot.h \
    toolboxscene.h

INCLUDEPATH += ../LIBS/common/

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ui.qrc
