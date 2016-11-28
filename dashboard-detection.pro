TEMPLATE = app
CONFIG += console
CONFIG -= qt

CONFIG += c++11

CONFIG += link_pkgconfig
PKGCONFIG += opencv

HEADERS += \
    hough.h \
    rescale.h \
    sobel.h \
    filter_list.h
    ground.h

SOURCES += \
    face.cpp \
    hough.cpp \
    rescale.cpp \
    sobel.cpp \
    filter_list.cpp
    rescale.cpp \
    ground.cpp
