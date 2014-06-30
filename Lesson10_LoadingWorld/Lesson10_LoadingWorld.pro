#-------------------------------------------------
#
# Project created by QtCreator 2014-06-28T21:52:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

target.path = /home/pi/NeHe_OpenGL_Qt5
TARGET = Lesson10_LoadingWorld
INSTALLS += target
TEMPLATE = app


SOURCES += main.cpp \
    openglwindow.cpp \
    worldwindow.cpp

HEADERS  += \
    openglwindow.h \
    worldwindow.h

RESOURCES += \
    src.qrc
