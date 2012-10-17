#-------------------------------------------------
#
# Project created by QtCreator 2012-10-17T11:12:09
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = TP4
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += include

SOURCES += \
    src/utils.cpp \
    src/main.cpp \
    src/Individuo.cpp \
    src/GNUPlot.cpp \
    src/Config.cpp \
    src/AlgoritmoGenetico.cpp

HEADERS += \
    include/utils.h \
    include/Individuo.h \
    include/GNUPlot.h \
    include/Config.h \
    include/AlgoritmoGenetico.h
