#-------------------------------------------------
#
# Project created by QtCreator 2012-10-16T12:12:56
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = TP4
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += src/main.cpp \
    src/utils.cpp \
    src/GNUPlot.cpp \
    src/Config.cpp \
    src/Individuo.cpp \
    src/AlgoritmoGenetico.cpp \
    src/AgenteViajero.cpp

INCLUDEPATH += include

HEADERS += \
    include/utils.h \
    include/GNUPlot.h \
    include/Config.h \
    include/AlgoritmoGenetico.h \
    include/Individuo.h \
    include/AgenteViajero.h \
    include/AgenteViajero.h
