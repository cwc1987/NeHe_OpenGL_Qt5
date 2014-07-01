#-------------------------------------------------
#
# Project created by QtCreator 2014-07-01T22:37:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

target.path = /home/pi/NeHe_OpenGL_Qt5
TARGET = Lesson14_OutlineFonts
INSTALLS += target
TEMPLATE = app

LIBS += libfreetype libz

SOURCES += main.cpp \
    openglwindow.cpp \
    fontwindow.cpp

HEADERS  += \
    openglwindow.h \
    fontwindow.h

OTHER_FILES += \
    shader/vertshader.glsl \
    shader/fragshader.glsl

RESOURCES += \
    src.qrc
