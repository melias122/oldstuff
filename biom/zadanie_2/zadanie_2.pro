QT += core
QT -= gui

TARGET = zadanie_2
CONFIG += console c++11 link_pkgconfig
CONFIG -= app_bundle

PKGCONFIG += opencv
LIBS += -L/usr/local/share/OpenCV/3rdparty/lib/ -lippicv

TEMPLATE = app

SOURCES += main.cpp \
    bayes.cpp

HEADERS += \
    bayes.h
