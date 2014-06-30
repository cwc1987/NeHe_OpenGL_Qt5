#-------------------------------------------------
#
# Project created by QtCreator 2014-06-28T18:52:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

target.path = /home/pi/NeHe_OpenGL_Qt5
TARGET = Lesson09_MovingScenes
INSTALLS += target
TEMPLATE = app


SOURCES += main.cpp\
    openglwindow.cpp \
    moveingsceneswindow.cpp

HEADERS += openglwindow.h \
    moveingsceneswindow.h

RESOURCES += \
    src.qrc
