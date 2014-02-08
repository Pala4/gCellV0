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

LIBS += $$sprintf(%1%2%3%4%5, -L, $$DEST_API_DIR, -l, $$PRJ_TARGET, _lib) #gCellV0_lib

SOURCES += main.cpp \
    cconio.cpp

HEADERS += \
    cconio.h