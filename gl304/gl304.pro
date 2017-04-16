QT += core gui opengl

TARGET = gl304
CONFIG += c++11

TEMPLATE = app

SOURCES += main.cpp \
    glwindow.cpp \
    scene.cpp \
    box.cpp \
    plot.cpp
HEADERS += \
    glwindow.h \
    vertex.h \
    scene.h \
    box.h \
    plot.h

win32 {
    LIBS += opengl32.lib
}

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all APIs deprecated before Qt 6.0.0

RESOURCES += \
    resources.qrc

DISTFILES +=
