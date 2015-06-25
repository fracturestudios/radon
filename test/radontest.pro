
QT += core network

TARGET = radontest
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

CONFIG += c++11

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CFLAGS_RELEASE += -O3

INCLUDEPATH += ../include
LIBS += -L../src -lradon

SOURCES += main.cpp             \
           historytests.cpp     \
           inboundtests.cpp     \
           retransmittests.cpp  \

HEADERS += test.h               \
           historytests.h       \
           inboundtests.h       \
           retransmittests.h    \

