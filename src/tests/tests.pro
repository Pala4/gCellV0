#-------------------------------------------------
#
# Project created by QtCreator 2014-02-17T22:41:13
#
#-------------------------------------------------
include("../gcellv0.pri")

QT       += network testlib
QT       -= gui

TEMPLATE = app
CONFIG   += console
CONFIG   -= app_bundle
DESTDIR = $$DEST_DIR
TARGET = $$join(TARGET,,, _tst)

DEFINES += SRCDIR=\\\"$$PWD/\\\"

#gCellV0_lib
INCLUDEPATH += $${PWD}/../GCellCoreLib
LIBS += -L $$DEST_API_DIR -l$$join(PRJ_TARGET,,, _lib)


SOURCES += tst_testscorelib.cpp \
    cmockcmdreceiver.cpp

HEADERS += \
    cmockcmdreceiver.h