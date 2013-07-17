#-------------------------------------------------
#
# Project created by QtCreator 2013-07-13T12:02:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GCellV0
TEMPLATE = app

DESTDIR = ../bin

SOURCES += main.cpp \
    core/engine/cengine.cpp \
    core/scheme/cscheme.cpp \
    gui/schemeview/cschememodel.cpp \
    gui/mainwindow/cmainwindow.cpp \
    gui/schemeview/cschemeview.cpp \
    gui/schemeview/cgrelement.cpp \
    core/scheme/celement.cpp \
    core/scheme/clink.cpp \
    gui/schemeview/cgrlink.cpp \
    elements/CSV/CSVIn/ccsvin.cpp \
    gui/schemeview/celementprotomng.cpp \
    gui/schemeview/celementproto.cpp \
    gui/mainwindow/celementprotostoolbar.cpp

HEADERS  += \
    core/engine/cengine.h \
    core/scheme/cscheme.h \
    gui/schemeview/cschememodel.h \
    gui/mainwindow/cmainwindow.h \
    gui/schemeview/cschemeview.h \
    gui/schemeview/cgrelement.h \
    core/scheme/celement.h \
    core/scheme/clink.h \
    gui/schemeview/cgrlink.h \
    elements/CSV/CSVIn/ccsvin.h \
    gui/schemeview/celementprotomng.h \
    gui/schemeview/celementproto.h \
    gui/mainwindow/celementprotostoolbar.h
