#-------------------------------------------------
#
# Project created by QtCreator 2014-02-08T11:06:56
#
#-------------------------------------------------
include("../gcellv0.pri")

QT -= gui
QT += core

TEMPLATE = lib
DLLDESTDIR = $$DEST_DIR
DESTDIR = $$DEST_API_DIR
TARGET = $$join(TARGET,,, _lib)

DEFINES += GCELLCORELIB_LIBRARY

SOURCES += \
    ciosystem.cpp \
    cchannel.cpp \
    cthreadmng.cpp \
    cbase.cpp \
    ctransaction.cpp \
    ctransactionevent.cpp \
    ctransactionprocessor.cpp

HEADERS +=\
        gcellcorelib_global.h \
    ciosystem.h \
    cchannel.h \
    cthreadmng.h \
    cbase.h \
    ctransaction.h \
    ctransactionevent.h \
    ctransactionprocessor.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}