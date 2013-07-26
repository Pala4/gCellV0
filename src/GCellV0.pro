#-------------------------------------------------
#
# Project created by QtCreator 2013-07-13T12:02:00
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GCellV0
TEMPLATE = app

DESTDIR = ../bin

SOURCES += main.cpp \
    mainwindow/cmainwindow.cpp \
    scheme/cscheme.cpp \
    scheme/cschemeeditor.cpp \
    engine/cengine.cpp \
    scheme/celement.cpp \
    scheme/cportal.cpp \
    scheme/clink.cpp \
    scheme/calgorithm.cpp \
    scheme/calgorithmproto.cpp \
    scheme/calgorithmprotomng.cpp \
    mainwindow/calgorithmprotostoolbar.cpp \
    algorithms/CSV/CSVIn/ccsvin.cpp \
    scheme/cargument.cpp \
    scheme/cresult.cpp \
    algorithms/General/Amp/camp.cpp \
    algorithms/CSV/CSVOut/ccsvout.cpp \
    scheme/cdatasource.cpp \
    scheme/cdatatransmitter.cpp \
    scheme/cdatareceiver.cpp \
    scheme/cxmlscheme.cpp

HEADERS  += \
    mainwindow/cmainwindow.h \
    scheme/cscheme.h \
    scheme/cschemeeditor.h \
    engine/cengine.h \
    scheme/celement.h \
    scheme/cportal.h \
    scheme/clink.h \
    scheme/calgorithm.h \
    scheme/calgorithmproto.h \
    scheme/calgorithmprotomng.h \
    mainwindow/calgorithmprotostoolbar.h \
    algorithms/CSV/CSVIn/ccsvin.h \
    scheme/cargument.h \
    scheme/cresult.h \
    algorithms/General/Amp/camp.h \
    algorithms/CSV/CSVOut/ccsvout.h \
    scheme/elementlistutil.h \
    scheme/cdatasource.h \
    scheme/cdatatransmitter.h \
    scheme/cdatareceiver.h \
    scheme/cxmlscheme.h
