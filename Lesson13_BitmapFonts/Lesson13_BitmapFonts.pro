#-------------------------------------------------
#
# Project created by QtCreator 2014-07-01T11:31:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lesson13_BitmapFonts
TEMPLATE = app

LIBS += libfreetype libz

SOURCES += main.cpp \
    openglwindow.cpp \
    fontwindow.cpp

HEADERS  += \
    openglwindow.h \
    fontwindow.h

RESOURCES += \
    src.qrc
