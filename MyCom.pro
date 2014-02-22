#-------------------------------------------------
#
# Project created by QtCreator 2014-01-28T14:27:50
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyCom
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    handleserialdata.cpp \
    qextserialport/qextserialport_win.cpp \
    qextserialport/qextserialport.cpp \
    nehewidget/nehewidget.cpp

HEADERS  += widget.h \
    handleserialdata.h \
    qextserialport/qextserialport_p.h \
    qextserialport/qextserialport_global.h \
    qextserialport/qextserialport.h \
    nehewidget/nehewidget.h

FORMS    += widget.ui \
    nehewidget/nehewidget.ui

RESOURCES += \
    nehewidget/image.qrc
