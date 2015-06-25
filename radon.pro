
QT += core network

TARGET = radon
TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CFLAGS_RELEASE += -O3

INCLUDEPATH += include

SOURCES += src/endpoint.cpp         \
           src/history.cpp          \
           src/inbound.cpp          \
           src/retransmit.cpp       \
           src/socket.cpp           \

HEADERS += include/rn/endpoint.h    \
           include/rn/header.h      \
           include/rn/history.h     \
           include/rn/inbound.h     \
           include/rn/retransmit.h  \
           include/rn/socket.h      \

