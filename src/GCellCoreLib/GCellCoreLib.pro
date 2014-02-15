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

INCLUDEPATH += \
    iosystem \
    threads \
    transaction \
    network

SOURCES += \
    cbase.cpp \
    iosystem/cchannel.cpp \
    iosystem/ciosystem.cpp \
    threads/cthreadmng.cpp \
    transaction/ctransaction.cpp \
    transaction/ctransactionevent.cpp \
    transaction/ctransactionprocessor.cpp

HEADERS +=\
    gcellcorelib_global.h \
    cbase.h \
    iosystem/cchannel.h \
    iosystem/ciosystem.h \
    threads/cthreadmng.h \
    transaction/ctransaction.h \
    transaction/ctransactionevent.h \
    transaction/ctransactionprocessor.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}