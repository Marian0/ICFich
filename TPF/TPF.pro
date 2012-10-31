#-------------------------------------------------
#
# Project created by QtCreator 2012-10-30T19:14:32
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = TPF
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
INCLUDEPATH += include


SOURCES += \
    src/main.cpp \
    src/utils.cpp \
    src/Individuo.cpp \
    src/GNUPlot.cpp \
    src/Config.cpp \
    src/AlgoritmoGenetico.cpp \
    src/Clase.cpp

HEADERS += \
    include/utils.h \
    include/Individuo.h \
    include/GNUPlot.h \
    include/Config.h \
    include/AlgoritmoGenetico.h \
    include/Clase.h

OTHER_FILES += \
    bin/configuracion.cfg
