TEMPLATE = app
CONFIG += console
CONFIG -= qt

CONFIG += c++11

CONFIG += link_pkgconfig
PKGCONFIG += opencv

HEADERS += \
    rescale.h \
    sobel.h \
    namedimage.h \
    hough.h \
    ground.h \
    dynamicthreshold.h \
    filterlist.h \
    detectanddisplay.h \
    calcf1.h \
    inrange.h \
    rectrange.h \
    lineintersections.h \
    samplevector.h

SOURCES += \
    face.cpp \
    rescale.cpp \
    sobel.cpp \
    namedimage.cpp \
    hough.cpp \
    ground.cpp \
    dynamicthreshold.cpp \
    filterlist.cpp \
    detectanddisplay.cpp \
    calcf1.cpp \
    rectrange.cpp \
    lineintersections.cpp
