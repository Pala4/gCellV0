#-------------------------------------------------
#
# Project created by QtCreator 2013-07-13T12:02:00
#
#-------------------------------------------------
include("../gcellv0.pri")

QT      +=  core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
DESTDIR = $$DEST_DIR
TARGET = $$join(TARGET,,, _gui)

LIBS += $$sprintf(%1%2%3%4%5, -L, $$DEST_API_DIR, -l, $$PRJ_TARGET, _lib) \ #gCellV0_lib
        $$sprintf(%1%2, -lqwt, $$replace(DR_POSTFIX, _,)) #qwtlib

INCLUDEPATH += \
    scheme \
    engine \
    threadengine \
    mainwindow

SOURCES += \
    mainwindow/cmainwindow.cpp \
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
    algorithms/TAC/TransLink/ctranslink.cpp \
    algorithms/TAC/TransLink/ctlabstractalgorithm.cpp \
    algorithms/TAC/TransLink/cdiscretdiffmethod.cpp \
    mainwindow/coptionswindow.cpp \
    mainwindow/calgprotoview.cpp \
    widgets/clineeditbutton.cpp \
    widgets/ccolorlineedit.cpp \
    scheme/link/clinksegment.cpp \
    scheme/link/clinkenv.cpp \
    scheme/link/csegmentmover.cpp \
    scheme/cgrid.cpp \
    scheme/cbounds.cpp \
    mainwindow/csavemodschemesdlg.cpp \
    mainwindow/datawindow/algbuffmodel/calgbuffmodelitem.cpp \
    mainwindow/datawindow/algbuffmodel/calgtreemodel.cpp \
    mainwindow/workspace/cdocument.cpp \
    mainwindow/workspace/cworkspace.cpp \
    mainwindow/workspace/cschemedocument.cpp \
    mainwindow/workspace/iview.cpp \
    objectmodel/cobjectitem.cpp \
    objectmodel/cobjectmodel.cpp \
    threadengine/cthreadengine.cpp \
    threadengine/cthreadtask.cpp

HEADERS += \
    mainwindow/cmainwindow.h \
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
    algorithms/TAC/TransLink/ctranslink.h \
    algorithms/TAC/TransLink/ctlabstractalgorithm.h \
    algorithms/TAC/TransLink/cdiscretdiffmethod.h \
    mainwindow/coptionswindow.h \
    mainwindow/calgprotoview.h \
    widgets/clineeditbutton.h \
    widgets/ccolorlineedit.h \
    scheme/link/clinksegment.h \
    scheme/link/clinkenv.h \
    scheme/link/csegmentmover.h \
    scheme/cgrid.h \
    scheme/cbounds.h \
    mainwindow/csavemodschemesdlg.h \
    mainwindow/datawindow/algbuffmodel/calgbuffmodelitem.h \
    mainwindow/datawindow/algbuffmodel/calgtreemodel.h \
    mainwindow/workspace/cdocument.h \
    mainwindow/workspace/cworkspace.h \
    mainwindow/workspace/cschemedocument.h \
    mainwindow/workspace/iview.h \
    objectmodel/cobjectitem.h \
    objectmodel/cobjectmodel.h \
    threadengine/cthreadengine.h \
    threadengine/cthreadtask.h