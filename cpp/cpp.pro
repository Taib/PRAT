TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    hypergraph.cpp \
    utils.cpp \
    hyperdenoise.cpp \
    set.cpp \
    hypermorpho.cpp

HEADERS += \
    HyperGraph.h \
    IANH.h \
    HyperDenoise.h \
    HyperMorpho.h \
    Set.h \
    Utils.h

LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui
