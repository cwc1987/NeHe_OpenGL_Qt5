#-------------------------------------------------
#
# Project created by QtCreator 2014-06-30T20:40:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

target.path = /home/pi/NeHe_OpenGL_Qt5
TARGET = Lesson12_DisplayLists
INSTALLS += target
TEMPLATE = app


SOURCES += main.cpp \
    texturemappingwindow.cpp \
    openglwindow.cpp

HEADERS  += \
    texturemappingwindow.h \
    openglwindow.h

RESOURCES += \
    src.qrc
