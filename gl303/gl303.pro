QT += core gui opengl

TARGET = gl303
CONFIG += c++11

TEMPLATE = app

SOURCES += main.cpp \
    glwindow.cpp \
    transform3d.cpp \
    scene.cpp \
    object.cpp
HEADERS += \
    glwindow.h \
    vertex.h \
    transform3d.h \
    scene.h \
    object.h

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all APIs deprecated before Qt 6.0.0

RESOURCES += \
    resources.qrc
