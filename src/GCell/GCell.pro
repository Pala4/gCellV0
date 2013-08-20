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
    scheme/link/clink.cpp \
    scheme/portal/cresult.cpp \
    scheme/portal/cportal.cpp \
    scheme/portal/cargument.cpp \
    scheme/cschemeeditor.cpp \
    scheme/cscheme.cpp \
    scheme/celement.cpp \
    main.cpp \
    algorithms/General/Sum/csum.cpp \
    scheme/celementoptionswgt.cpp \
    algorithms/CSV/CSVIn/ccsvinoptionshit.cpp \
    scheme/varutil.cpp \
    mainwindow/datawindow/cdatawindow.cpp \
    mainwindow/datawindow/cdatatable.cpp \
    mainwindow/datawindow/cdataplot.cpp \
    algorithms/TAC/StepExaction/cstepexcitation.cpp \
    algorithms/TAC/TransLink/ctranslink.cpp

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
    scheme/link/clink.h \
    scheme/portal/cresult.h \
    scheme/portal/cportal.h \
    scheme/portal/cargument.h \
    scheme/elementlistutil.h \
    scheme/cschemeeditor.h \
    scheme/cscheme.h \
    scheme/celement.h \
    algorithms/General/Sum/csum.h \
    scheme/celementoptionswgt.h \
    algorithms/CSV/CSVIn/ccsvinoptionshit.h \
    scheme/varutil.h \
    mainwindow/datawindow/cdatawindow.h \
    mainwindow/datawindow/cdatatable.h \
    mainwindow/datawindow/cdataplot.h \
    algorithms/TAC/StepExaction/cstepexcitation.h \
    algorithms/TAC/TransLink/ctranslink.h