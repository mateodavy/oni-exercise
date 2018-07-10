#-------------------------------------------------
#
# Project created by QtCreator 2018-06-20T18:28:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qt-ONI
TEMPLATE = app
CONFIG += c++11

INCLUDEPATH += /usr/local/include/opencv

#LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui
#LIBS += -L/usr/local/lib -lopencv_core

LIBS += -L/usr/lib/x86_64-linux-gnu/ -lopencv_core -lopencv_contrib -lopencv_imgproc -lopencv_highgui

QT += widgets

SOURCES += main.cpp\
        mainwindow.cpp \
    scanimage.cpp \
    imagestats.cpp \
    sigraphicsitem.cpp \
    sigraphicscene.cpp \
    imageproc.cpp

HEADERS  += mainwindow.h \
    scanimage.h \
    config.h \
    types.h \
    imagestats.h \
    sigraphicsitem.h \
    sigraphicscene.h \
    imageproc.h

FORMS    += mainwindow.ui
