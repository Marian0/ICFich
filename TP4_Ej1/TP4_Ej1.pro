TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += \
    src/utils.cpp \
    src/main.cpp \
    src/Individuo.cpp \
    src/GNUPlot.cpp \
    src/Config.cpp \
    src/AlgoritmoGenetico.cpp \
    src/gradiente.cpp

INCLUDEPATH += include

HEADERS += \
    include/Individuo.h \
    include/GNUPlot.h \
    include/Config.h \
    include/AlgoritmoGenetico.h \
    include/utils.h \
    include/gradiente.h

OTHER_FILES += \
    bin/configuracion1c.cfg \
    bin/configuracion1b.cfg \
    bin/configuracion1a.cfg

