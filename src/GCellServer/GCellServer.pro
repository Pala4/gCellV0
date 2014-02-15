#-------------------------------------------------
#
# Project created by QtCreator 2014-02-15T15:53:01
#
#-------------------------------------------------
include("../gcellv0.pri")

QT       += core
QT       -= gui

TEMPLATE = app
CONFIG   += console
CONFIG   -= app_bundle
DESTDIR = $$DEST_DIR
TARGET = $$join(TARGET,,, _srv)

#gCellV0_lib
INCLUDEPATH += $${PWD}/../GCellCoreLib
LIBS += -L $$DEST_API_DIR -l$$join(PRJ_TARGET,,, _lib)

SOURCES += main.cpp