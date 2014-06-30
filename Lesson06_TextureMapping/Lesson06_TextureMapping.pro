#-------------------------------------------------
#
# Project created by QtCreator 2014-06-20T10:32:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

target.path = /home/pi/NeHe_OpenGL_Qt5
TARGET = Lesson06_TextureMapping
INSTALLS += target
TEMPLATE = app


SOURCES += main.cpp\
        openglwindow.cpp \
    texturemappingwindow.cpp

HEADERS  += openglwindow.h \
    texturemappingwindow.h

RESOURCES += \
    src.qrc
