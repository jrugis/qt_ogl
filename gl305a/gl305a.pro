QT += core gui opengl

TARGET = gl305a
CONFIG += c++11

TEMPLATE = app

SOURCES += main.cpp \
    glwindow.cpp \
    scene.cpp \
    plot.cpp \
    plotcross.cpp

HEADERS += \
    glwindow.h \
    scene.h \
    plot.h \
    constants.h \
    plotcross.h

INCLUDEPATH += \
    /usr/local/include

LIBS += \
    -lpari \
    -L/usr/local/lib \
    -v

win64 {
    LIBS += opengl64.lib
}

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all APIs deprecated before Qt 6.0.0

RESOURCES += \
    resources.qrc

DISTFILES +=
