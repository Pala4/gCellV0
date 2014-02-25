#-------------------------------------------------
#
# Project created by QtCreator 2014-02-08T11:06:56
#
#-------------------------------------------------
include("../gcellv0.pri")

QT += core network
QT -= gui

TEMPLATE = lib
DLLDESTDIR = $$DEST_DIR
DESTDIR = $$DEST_API_DIR
TARGET = $$join(TARGET,,, _lib)

DEFINES += GCELLCORELIB_LIBRARY

INCLUDEPATH += \
    command \
    engine \
    network

SOURCES += \
    network/cserver.cpp \
    network/csocket.cpp \
    network/csocketmng.cpp \
    command/ccommand.cpp \
    command/ccommanddsp.cpp \
    engine/cengine.cpp \
    engine/scheme/calg.cpp \
    engine/scheme/calgdescmng.cpp \
    engine/scheme/cportal.cpp \
    engine/scheme/cscheme.cpp \
    engine/scheme/cschememng.cpp \
    command/ccmdparser.cpp

HEADERS +=\
    network/cserver.h \
    network/csocket.h \
    network/csocketmng.h \
    command/ccommand.h \
    command/ccommanddsp.h \
    engine/cengine.h \
    engine/scheme/algdesc.h \
    engine/scheme/calg.h \
    engine/scheme/calgdescmng.h \
    engine/scheme/cportal.h \
    engine/scheme/cscheme.h \
    engine/scheme/cschememng.h \
    engine/scheme/listutil.h \
    engine/scheme/portaldesc.h \
    command/ccmdparser.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}