TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += \
    src/utils.cpp \
    src/Red.cpp \
    src/particula.cpp \
    src/Neurona.cpp \
    src/main.cpp \
    src/GNUPlot.cpp \
    src/enjambre.cpp \
    src/Config.cpp

INCLUDEPATH += include

HEADERS += \
    include/utils.h \
    include/Red.h \
    include/particula.h \
    include/Neurona.h \
    include/GNUPlot.h \
    include/enjambre.h \
    include/Config.h

