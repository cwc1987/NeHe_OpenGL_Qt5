#-------------------------------------------------
#
# Project created by QtCreator 2014-06-19T22:06:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

target.path = /home/pi/NeHe_OpenGL_Qt5
TARGET = Lesson03_SmoothColors
INSTALLS += target
TEMPLATE = app


SOURCES += main.cpp\
        openglwindow.cpp \
    polygonwindow.cpp

HEADERS  += openglwindow.h \
    polygonwindow.h

RESOURCES += \
    src.qrc
