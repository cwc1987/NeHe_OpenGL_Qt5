#-------------------------------------------------
#
# Project created by QtCreator 2014-06-10T15:02:22
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lesson02_CreatePolygon
TEMPLATE = app


SOURCES += main.cpp\
        openglwindow.cpp \
    trianglewindow.cpp \
    polygonwindow.cpp

HEADERS  += openglwindow.h \
    trianglewindow.h \
    polygonwindow.h
