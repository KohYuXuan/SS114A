#-------------------------------------------------
#
# Project created by QtCreator 2011-10-29T14:41:06
#
#-------------------------------------------------

QT       += core gui

TARGET = QTObfuscate
TEMPLATE = app
LIBS = -ldl
CONFIG += static
    win32 {
    QMAKE_LFLAGS += -static-libgcc
    }

SOURCES += main.cpp\
        mainwindow.cpp \
    FileManager.cpp \
    File.cpp \
    Engine.cpp \
    PluginManager.cpp \
    Plugin.cpp

HEADERS  += mainwindow.h \
    FileManager.h \
    File.h \
    Engine.h \
    PluginManager.h \
    Plugin.h

FORMS    += mainwindow.ui























