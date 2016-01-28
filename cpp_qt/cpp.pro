TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

HEADERS += \
    HyperGraph.h \
    IANH.h \
    HyperDenoise.h \
    HyperMorpho.h \
    Set.h \
    Utils.h \
    SLIC.h \
    SLIC_Hyper.h \
    ImageNeighborKernel.h \
    DistanceKernel.h \
    EMIHA.h \
    PixelHyperGraph.h \
    NLIH.h

LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui
