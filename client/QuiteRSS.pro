#-------------------------------------------------
#
# Project created by QtCreator 2018-10-05T12:31:18
#
#-------------------------------------------------

QT += core gui widgets websockets

TARGET = QuiteRSS
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/feedsview.cpp \
    src/categoriestreewidget.cpp \
    src/newstabwidget.cpp \
    src/tabbar.cpp \
    src/toolbutton.cpp \
    src/newsview.cpp \
    src/feedsmodel.cpp \
    src/delegatewithoutfocus.cpp \
    src/newsmodel.cpp \
    src/server.cpp \
    src/newsheader.cpp \
    src/settings.cpp \
    src/feeditem.cpp

HEADERS += \
    src/mainwindow.h \
    src/feedsview.h \
    src/categoriestreewidget.h \
    src/newstabwidget.h \
    src/tabbar.h \
    src/toolbutton.h \
    src/newsview.h \
    src/feedsmodel.h \
    src/delegatewithoutfocus.h \
    src/newsmodel.h \
    src/server.h \
    src/newsheader.h \
    src/settings.h \
    src/structs.h \
    src/structs.h \
    src/feeditem.h

RESOURCES += \
    quiterss.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
