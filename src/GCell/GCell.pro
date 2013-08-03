#-------------------------------------------------
#
# Project created by QtCreator 2013-07-13T12:02:00
#
#-------------------------------------------------
include("../gcellv0.pri")

QT      +=  core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG(release,debug|release){
    TARGET = gCellV0Gui
    LIBS    +=  -lqwt
}

CONFIG(debug,debug|release){
    TARGET = gCellV0Gui_d
    LIBS    +=  -lqwtd
}

TEMPLATE = app

DESTDIR = $$DEST_DIR

SOURCES += \
    mainwindow/cmainwindow.cpp \
    mainwindow/calgorithmprotostoolbar.cpp \
    algorithms/CSV/CSVIn/ccsvin.cpp \
    algorithms/CSV/CSVOut/ccsvout.cpp \
    algorithms/General/Amp/camp.cpp \
    engine/cengine.cpp \
    scheme/algorithm/cdatatransmitter.cpp \
    scheme/algorithm/cdatasource.cpp \
    scheme/algorithm/cdatareceiver.cpp \
    scheme/algorithm/calgorithm.cpp \
    scheme/algorithmproto/calgorithmprotomng.cpp \
    scheme/algorithmproto/calgorithmproto.cpp \
    scheme/databuffer/cdatabuffer.cpp \
    scheme/databuffer/cabstractdatabuffer.cpp \
    scheme/link/clink.cpp \
    scheme/portal/cresult.cpp \
    scheme/portal/cportal.cpp \
    scheme/portal/cargument.cpp \
    scheme/cxmlscheme.cpp \
    scheme/cschemeeditor.cpp \
    scheme/cscheme.cpp \
    scheme/celement.cpp \
    main.cpp \
    mainwindow/cdatawindow.cpp

HEADERS += \
    mainwindow/cmainwindow.h \
    mainwindow/calgorithmprotostoolbar.h \
    algorithms/CSV/CSVIn/ccsvin.h \
    algorithms/CSV/CSVOut/ccsvout.h \
    algorithms/General/Amp/camp.h \
    engine/cengine.h \
    scheme/algorithm/cdatatransmitter.h \
    scheme/algorithm/cdatasource.h \
    scheme/algorithm/cdatareceiver.h \
    scheme/algorithm/calgorithm.h \
    scheme/algorithmproto/calgorithmprotomng.h \
    scheme/algorithmproto/calgorithmproto.h \
    scheme/databuffer/cdatabuffer.h \
    scheme/databuffer/cabstractdatabuffer.h \
    scheme/link/clink.h \
    scheme/portal/cresult.h \
    scheme/portal/cportal.h \
    scheme/portal/cargument.h \
    scheme/elementlistutil.h \
    scheme/cxmlscheme.h \
    scheme/cschemeeditor.h \
    scheme/cscheme.h \
    scheme/celement.h \
    mainwindow/cdatawindow.h