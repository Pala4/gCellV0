#-------------------------------------------------
#
# Project created by QtCreator 2014-02-08T12:53:50
#
#-------------------------------------------------
include("../gcellv0.pri")

QT       += core
QT       -= gui

TEMPLATE = app
CONFIG   += console
CONFIG   -= app_bundle
DESTDIR = $$DEST_DIR
TARGET = $$join(TARGET,,, _con)

#gCellV0_lib
INCLUDEPATH += $${PWD}/../GCellCoreLib
LIBS += -L $$DEST_API_DIR -l$$join(PRJ_TARGET,,, _lib)

SOURCES += main.cpp \
    cconio.cpp \
    cconsoleapp.cpp

HEADERS += \
    cconio.h \
    cconsoleapp.h