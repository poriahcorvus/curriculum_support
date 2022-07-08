#-------------------------------------------------
#
# Project created by QtCreator 2022-05-29T15:06:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = imrenzhen
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    dealfile.cpp \
    document.cpp \
    down.cpp \
    hnode.cpp \
    homework.cpp \
    huffmancode_node.cpp \
        main.cpp \
    navdialog.cpp \
    studentwidget.cpp \
    course.cpp \
    quanju.cpp \
    courseku.cpp \
    updoc.cpp \
    widget.cpp \
    showcu.cpp \
    manager.cpp \
    timethread.cpp \
    myclock.cpp \
    myactivity.cpp \
    clockku.cpp \
    clockthread.cpp \
    clockmanager.cpp \
    msgbox.cpp \
    actku.cpp \
    actmanager.cpp \
    actplus.cpp \
    managerchange.cpp \
    courseplus.cpp \
    manahome.cpp \
    homeku.cpp \
    plushome.cpp \
    groupactku.cpp \
    groupact.cpp \
    plusgroupact.cpp \
    mapchoose.cpp

HEADERS += \
    dealfile.h \
    document.h \
    down.h \
    hnode.h \
    homework.h \
    huffmancode_node.h \
    navdialog.h \
    studentwidget.h \
    course.h \
    quanju.h \
    courseku.h \
    updoc.h \
    widget.h \
    showcu.h \
    manager.h \
    timethread.h \
    myclock.h \
    myactivity.h \
    clockku.h \
    clockthread.h \
    clockmanager.h \
    msgbox.h \
    actku.h \
    actmanager.h \
    actplus.h \
    managerchange.h \
    courseplus.h \
    manahome.h \
    homeku.h \
    plushome.h \
    groupactku.h \
    groupact.h \
    plusgroupact.h \
    mapchoose.h

FORMS += \
    document.ui \
    down.ui \
    homework.ui \
    navdialog.ui \
    updoc.ui \
        widget.ui \
    studentwidget.ui \
    showcu.ui \
    manager.ui \
    clockmanager.ui \
    actmanager.ui \
    actplus.ui \
    managerchange.ui \
    courseplus.ui \
    plushome.ui \
    plusgroupact.ui \
    mapchoose.ui

DISTFILES +=

RESOURCES += \
    res.qrc
