TEMPLATE = app
CONFIG += console
CONFIG -= qt

CONFIG += c++11

CONFIG += link_pkgconfig
PKGCONFIG += opencv

HEADERS += \
    rescale.h \
    sobel.h \
    filter_list.h \
    namedimage.h \
    hough.h \
    dynamic_threshold.h

SOURCES += \
    face.cpp \
    rescale.cpp \
    sobel.cpp \
    filter_list.cpp \
    namedimage.cpp \
    hough.cpp \
    dynamic_threshold.cpp
