#include "cmainwindow.h"

#include <QApplication>
#include <QMimeData>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QEvent>
#include <QMouseEvent>
#include <QSettings>
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include <QDockWidget>
#include <QStatusBar>
#include <QLabel>
#include <QSplitter>

#include "coptionswindow.h"
#include "datawindow/cdatawindow.h"
#include "csavemodschemesdlg.h"
#include "elementlistutil.h"
#include "algorithmproto/calgorithmproto.h"
#include "algorithmproto/calgorithmprotomng.h"
#include "calgprotoview.h"
#include "cscheme.h"
#include "cschemeeditor.h"
#include "cthreadengine.h"
#include "../algorithms/General/Amp/camp.h"
#include "../algorithms/General/Sum/csum.h"
#include "../algorithms/CSV/CSVIn/ccsvin.h"
#include "../algorithms/CSV/CSVOut/ccsvout.h"
#include "../algorithms/TAC/StepExaction/cstepexcitation.h"
#include "../algorithms/TAC/TransLink/ctranslink.h"

/*!
 * \class CMainWindow
 */
void CMainWindow::setupActions(void)
{
    //File
    m_acNewScheme = new QAction(tr("New scheme"), this);
    m_acNewScheme->setObjectName(QStringLiteral("acNewScheme"));
    connect(m_acNewScheme, SIGNAL(triggered()), this, SLOT(newScheme()));

    m_acOpenScheme = new QAction(tr("Open scheme..."), this);
    m_acOpenScheme->setObjectName(QStringLiteral("acOpenScheme"));
    connect(m_acOpenScheme, SIGNAL(triggered()), this, SLOT(openScheme()));

    m_acSaveScheme = new QAction(tr("Save scheme"), this);
    m_acSaveScheme->setObjectName(QStringLiteral("acSaveScheme"));
    connect(m_acSaveScheme, SIGNAL(triggered()), this, SLOT(saveScheme()));

    m_acSaveSchemeAs = new QAction(tr("Save scheme as..."), this);
    m_acSaveSchemeAs->setObjectName(QStringLiteral("acSaveSchemeAs"));
    connect(m_acSaveSchemeAs, SIGNAL(triggered()), this, SLOT(saveSchemeAs()));

    m_acCloseScheme = new QAction(tr("Close scheme"), this);
    m_acCloseScheme->setObjectName(QStringLiteral("acCloseScheme"));
    connect(m_acCloseScheme, SIGNAL(triggered()), this, SLOT(closeScheme()));

    m_acQuit = new QAction(tr("Quit"), this);
    m_acQuit->setObjectName(QStringLiteral("acQuit"));
    connect(m_acQuit, SIGNAL(triggered()), this, SLOT(close()));

    //Edit
    m_acGrSchemeMouseMode = new QActionGroup(this);
    m_acGrSchemeMouseMode->setObjectName(QStringLiteral("acGrSchemeMouseMode"));
    m_acGrSchemeMouseMode->setExclusive(true);

    m_acCursor = new QAction(tr("Cursor"), this);
    m_acCursor->setObjectName(QStringLiteral("acCursor"));
    m_acCursor->setCheckable(true);
    m_acCursor->setEnabled(false);
    connect(m_acCursor, SIGNAL(triggered(bool)), this, SLOT(onCursorTriggered(bool)));
    m_acCursor->trigger();
    m_acGrSchemeMouseMode->addAction(m_acCursor);

    m_acHand = new QAction(tr("Hand"), this);
    m_acHand->setObjectName(QStringLiteral("acHand"));
    m_acHand->setCheckable(true);
    m_acHand->setEnabled(false);
    connect(m_acHand, SIGNAL(triggered(bool)), this, SLOT(onHandTriggered(bool)));
    m_acGrSchemeMouseMode->addAction(m_acHand);

    m_acLinking = new QAction(tr("Link"), this);
    m_acLinking->setObjectName(QStringLiteral("acLinking"));
    m_acLinking->setCheckable(true);
    m_acLinking->setEnabled(false);
    connect(m_acLinking, SIGNAL(triggered(bool)), this, SLOT(onLinkingTriggered(bool)));
    m_acGrSchemeMouseMode->addAction(m_acLinking);

    m_acEditSep = new QAction(this);
    m_acEditSep->setSeparator(true);
    m_acEditSep->setVisible(false);

    m_acCopy = new QAction(tr("Copy"), this);
    m_acCopy->setObjectName(QStringLiteral("acCopy"));
    m_acCopy->setShortcut(QKeySequence::Copy);
    m_acCopy->setShortcutContext(Qt::WindowShortcut);
    m_acCopy->setEnabled(false);
    m_acCopy->setVisible(false);
    connect(m_acCopy, SIGNAL(triggered()), this, SLOT(copy()));

    m_acCut = new QAction(tr("Cut"), this);
    m_acCut->setObjectName(QStringLiteral("acCut"));
    m_acCut->setShortcut(QKeySequence::Cut);
    m_acCut->setShortcutContext(Qt::WindowShortcut);
    m_acCut->setEnabled(false);
    m_acCut->setVisible(false);
    connect(m_acCut, SIGNAL(triggered()), this, SLOT(cut()));

    m_acDelete = new QAction(tr("Delete"), this);
    m_acDelete->setObjectName(QStringLiteral("acDelete"));
    m_acDelete->setShortcut(QKeySequence::Delete);
    m_acDelete->setShortcutContext(Qt::WindowShortcut);
    m_acDelete->setEnabled(false);
    m_acDelete->setVisible(false);
    connect(m_acDelete, SIGNAL(triggered()), this, SLOT(del()));

    m_acPaste = new QAction(tr("Paste"), this);
    m_acPaste->setObjectName(QStringLiteral("acPaste"));
    m_acPaste->setShortcut(QKeySequence::Paste);
    m_acPaste->setShortcutContext(Qt::WindowShortcut);
    m_acPaste->setEnabled(false);
    m_acPaste->setVisible(false);
    connect(m_acPaste, SIGNAL(triggered()), this, SLOT(paste()));

    //View
    m_acDataWindow = new QAction(tr("Data window..."), this);
    m_acDataWindow->setObjectName(QStringLiteral("acDataWindow"));
    m_acDataWindow->setCheckable(true);
    connect(m_acDataWindow, SIGNAL(triggered(bool)), this, SLOT(setDataWindowVisible(bool)));

    //Calc active scheme
    m_acCalcActiveScheme = new QAction(tr("Calc active scheme"), this);
    m_acCalcActiveScheme->setObjectName(QStringLiteral("acCalcActiveScheme"));
    m_acCalcActiveScheme->setEnabled(false);
    connect(m_acCalcActiveScheme, SIGNAL(triggered()), this, SLOT(calcActiveScheme()));

    //Calc all schemes
    m_acCalcAllSchemes = new QAction(tr("Calc all schemes"), this);
    m_acCalcAllSchemes->setObjectName(QStringLiteral("acCalcAllSchemes"));
    m_acCalcAllSchemes->setEnabled(false);
    connect(m_acCalcAllSchemes, SIGNAL(triggered()), this, SLOT(calcAllSchemes()));

    //Options
    m_acOptions = new QAction(tr("Options..."), this);
    m_acOptions->setObjectName(QStringLiteral("acOptions"));
    connect(m_acOptions, SIGNAL(triggered()), this, SLOT(showOptions()));

    m_acElementOptions = new QAction(tr("Element options..."), this);
    m_acElementOptions->setObjectName(QStringLiteral("acElementOptions"));
    m_acElementOptions->setEnabled(false);
    m_acElementOptions->setVisible(false);
    connect(m_acElementOptions, SIGNAL(triggered()), this, SLOT(elementOptions()));
}

void CMainWindow::setupMainMenu(void)
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    if (m_acNewScheme != nullptr)
        fileMenu->addAction(m_acNewScheme);
    if (m_acOpenScheme != nullptr) {
        fileMenu->addAction(m_acOpenScheme);
        fileMenu->addSeparator();
    }
    if (m_acSaveScheme != nullptr)
        fileMenu->addAction(m_acSaveScheme);
    if (m_acSaveSchemeAs != nullptr) {
        fileMenu->addAction(m_acSaveSchemeAs);
        fileMenu->addSeparator();
    }
    if (m_acCloseScheme != nullptr) {
        fileMenu->addAction(m_acCloseScheme);
        fileMenu->addSeparator();
    }
    if (m_acQuit != nullptr)
        fileMenu->addAction(m_acQuit);

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    if (m_acGrSchemeMouseMode != nullptr)
        editMenu->addActions(m_acGrSchemeMouseMode->actions());
    if (m_acEditSep != nullptr)
        editMenu->addAction(m_acEditSep);
    if (m_acCopy != nullptr)
        editMenu->addAction(m_acCopy);
    if (m_acCut != nullptr)
        editMenu->addAction(m_acCut);
    if (m_acDelete != nullptr)
        editMenu->addAction(m_acDelete);
    if (m_acPaste != nullptr)
        editMenu->addAction(m_acPaste);

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    if (m_acDataWindow != nullptr)
        viewMenu->addAction(m_acDataWindow);
    m_viewToolBarsMenu = viewMenu->addMenu(tr("Tool bars"));
    m_viewToolBarsMenu->setObjectName("viewToolBarsMenu");
    m_viewDocksMenu = viewMenu->addMenu(tr("Docks"));
    m_viewDocksMenu->setObjectName(QStringLiteral("viewDocksMenu"));

    QMenu *calcMenu = menuBar()->addMenu(tr("&Calculate"));
    if (m_acCalcActiveScheme != nullptr)
        calcMenu->addAction(m_acCalcActiveScheme);
    if (m_acCalcAllSchemes != nullptr)
        calcMenu->addAction(m_acCalcAllSchemes);

    //	menuBar()->addMenu(tr("&Algorithms"));

    QMenu *optionsMenu = menuBar()->addMenu(tr("&Options"));
    if (m_acOptions != nullptr)
        optionsMenu->addAction(m_acOptions);
    if (m_acElementOptions != nullptr)
        optionsMenu->addAction(m_acElementOptions);

    menuBar()->addMenu(tr("&Help"));
}

void CMainWindow::setupToolBars(void)
{
    //File
    QToolBar *tbFile = addToolBar(tr("File"));
    tbFile->setObjectName(QStringLiteral("tbMain"));
    if (m_viewToolBarsMenu != nullptr)
        m_viewToolBarsMenu->addAction(tbFile->toggleViewAction());
    if (m_acNewScheme != nullptr)
        tbFile->addAction(m_acNewScheme);
    if (m_acOpenScheme != nullptr) {
        tbFile->addAction(m_acOpenScheme);
        tbFile->addSeparator();
    }
    if (m_acSaveScheme != nullptr)
        tbFile->addAction(m_acSaveScheme);
    if (m_acSaveSchemeAs != nullptr) {
        tbFile->addAction(m_acSaveSchemeAs);
        tbFile->addSeparator();
    }
    if (m_acCloseScheme != nullptr) {
        tbFile->addAction(m_acCloseScheme);
        tbFile->addSeparator();
    }
    if (m_acQuit != nullptr)
        tbFile->addAction(m_acQuit);

    //Edit
    addToolBarBreak();
    QToolBar *tbEdit = addToolBar(tr("Edit"));
    tbEdit->setObjectName(QStringLiteral("tbEdit"));
    if (m_viewToolBarsMenu != nullptr)
        m_viewToolBarsMenu->addAction(tbEdit->toggleViewAction());
    if (m_acGrSchemeMouseMode != nullptr)
        tbEdit->addActions(m_acGrSchemeMouseMode->actions());
    if (m_acEditSep != nullptr)
        tbEdit->addAction(m_acEditSep);
    if (m_acCopy != nullptr)
        tbEdit->addAction(m_acCopy);
    if (m_acCut != nullptr)
        tbEdit->addAction(m_acCut);
    if (m_acDelete != nullptr)
        tbEdit->addAction(m_acDelete);

    //View
    QToolBar *tbView = addToolBar(tr("View"));
    tbView->setObjectName(QStringLiteral("tbView"));
    if (m_viewToolBarsMenu != nullptr)
        m_viewToolBarsMenu->addAction(tbView->toggleViewAction());
    if (m_acDataWindow != nullptr)
        tbView->addAction(m_acDataWindow);

    //Calc
    QToolBar *tbCalc = addToolBar(tr("Calculation"));
    tbCalc->setObjectName(QStringLiteral("tbCalc"));
    if (m_viewToolBarsMenu != nullptr)
        m_viewToolBarsMenu->addAction(tbCalc->toggleViewAction());
    if (m_acCalcActiveScheme != nullptr)
        tbCalc->addAction(m_acCalcActiveScheme);
    if (m_acCalcAllSchemes != nullptr)
        tbCalc->addAction(m_acCalcAllSchemes);

    //Options
    QToolBar *tbOptions = addToolBar(tr("Options"));
    tbOptions->setObjectName(QStringLiteral("tbOptions"));
    if (m_viewToolBarsMenu != nullptr)
        m_viewToolBarsMenu->addAction(tbOptions->toggleViewAction());
    if (m_acOptions != nullptr)
        tbOptions->addAction(m_acOptions);
}

void CMainWindow::setupDocks(void)
{
    if (m_algProtoView != nullptr) {
        m_algProtoViewDock = new QDockWidget(m_algProtoView->windowTitle());
        m_algProtoViewDock->setObjectName(QStringLiteral("algProtoViewDock"));
        m_algProtoViewDock->setAllowedAreas(Qt::AllDockWidgetAreas);
        m_algProtoViewDock->setWidget(m_algProtoView);
        m_algProtoViewDock->setTitleBarWidget(new QLabel(""));
        if (m_viewDocksMenu != nullptr)
            m_viewDocksMenu->addAction(m_algProtoViewDock->toggleViewAction());
        connect(m_algProtoViewDock, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)),
                this, SLOT(onAlgProtosViewDockLocationChanged(Qt::DockWidgetArea)));
        connect(m_algProtoViewDock, SIGNAL(topLevelChanged(bool)),
                this, SLOT(onAlgProtosViewDockTopLevelChanged(bool)));
        addDockWidget(Qt::TopDockWidgetArea, m_algProtoViewDock);
    }
}

void CMainWindow::setupStatusBar(void)
{
    statusBar()->showMessage(tr("Ready"));
}

void CMainWindow::setupSchemeEditorContextMenu(void)
{
    if (m_schemeEditorContextMenu == nullptr) {
        m_schemeEditorContextMenu = new QMenu();
        m_schemeEditorContextMenu->setObjectName(QStringLiteral("schemeEditorContextMenu"));
    }

    m_schemeEditorContextMenu->clear();

    if (m_acElementOptions != nullptr)
        m_schemeEditorContextMenu->addAction(m_acElementOptions);

    CSchemeEditor *schemeEditor = activeSchemeEditor();
    if ((schemeEditor != nullptr) && (schemeEditor->selectedElements().count() == 1)) {
        if (!schemeEditor->selectedElements().at(0)->actions().isEmpty()) {
            if (!m_schemeEditorContextMenu->isEmpty())
                m_schemeEditorContextMenu->addSeparator();
            QList<QAction*> elementsActions = schemeEditor->selectedElements().at(0)->actions();
            m_schemeEditorContextMenu->addActions(elementsActions);
        }
    } else {
        if (m_acCursor != nullptr) {
            if (!m_schemeEditorContextMenu->isEmpty())
                m_schemeEditorContextMenu->addSeparator();
            m_schemeEditorContextMenu->addAction(m_acCursor);
        }
        if (m_acHand != nullptr)
            m_schemeEditorContextMenu->addAction(m_acHand);
        if (m_acLinking != nullptr)
            m_schemeEditorContextMenu->addAction(m_acLinking);
    }
    if (m_acEditSep != nullptr)
        m_schemeEditorContextMenu->addAction(m_acEditSep);
    if (m_acCopy != nullptr)
        m_schemeEditorContextMenu->addAction(m_acCopy);
    if (m_acCut != nullptr)
        m_schemeEditorContextMenu->addAction(m_acCut);
    if (m_acDelete != nullptr)
        m_schemeEditorContextMenu->addAction(m_acDelete);
}

void CMainWindow::writeScheme(CScheme *scheme, const QString &fileName)
{
    if ((scheme == nullptr) || fileName.isEmpty())
        return;

    QFile fileHandler(fileName);
    if (!fileHandler.open(QIODevice::WriteOnly))
        return;

    stSchemeDesc schemeDesc;
    schemeDesc.width = scheme->sceneRect().width();
    schemeDesc.height = scheme->sceneRect().height();

    QTextStream(&fileHandler) << scheme->toXMLDom(scheme->elements(), &schemeDesc).toString();
    fileHandler.close();

    scheme->setNewScheme(false);
    scheme->setModified(false);
}

bool CMainWindow::readScheme(CScheme *scheme, const QString &fileName)
{
    if ((scheme == nullptr) || fileName.isEmpty())
        return false;

    QFile fileHandler(fileName);
    if (!fileHandler.exists())
        return false;

    QDomDocument domDoc;
    QString errMsg;
    int errLine = 0;
    int errCol = 0;
    if (!domDoc.setContent(&fileHandler, &errMsg, &errLine, &errCol))
        return false;

    stSchemeDesc schemeDesc;
    QList<CElement*> elements = scheme->fromXMLDom(domDoc, &schemeDesc);
    scheme->setSceneRect(0.0, 0.0, schemeDesc.width, schemeDesc.height);
    scheme->addElements(elements);

    scheme->setNewScheme(false);
    scheme->setModified(false);

    return true;
}

bool CMainWindow::saveSchemesBeforeClose(const QList<CScheme*> &schemes)
{
    QList<CScheme*> modSchemes;

    foreach (CScheme *scheme, schemes) {
        if (scheme == nullptr)
            continue;

        if (scheme->isModified() &&
                (scheme->isNewScheme() || (!scheme->isNewScheme() && !isAutoSaveLastScheme()))) {
            modSchemes << scheme;
        } else if (!scheme->isNewScheme() && isAutoSaveLastScheme()) {
            if(scheme->isModified()) saveScheme(scheme);
        }
    }

    if (!modSchemes.isEmpty()) {
        CSaveModSchemesDlg saveModDlg;

        saveModDlg.setSavedSchemes(modSchemes);
        int execResult = saveModDlg.exec();
        switch (execResult) {
        case QDialog::Accepted: {
            QList<stSavedScheme> savedSchemes = saveModDlg.savedSchemes();
            foreach (stSavedScheme savedScheme, savedSchemes) {
                if ((savedScheme.scheme == nullptr) || !savedScheme.canBeStored)
                    continue;

                saveScheme(savedScheme.scheme);
            }
            return true;
        }
        case QDialog::Rejected: return false;
        case CSaveModSchemesDlg::Discarded: return true;
        }
    }

    return true;
}

CScheme* CMainWindow::activeScheme(void)
{
    return m_activeScheme;
}

CSchemeEditor* CMainWindow::activeSchemeEditor(void)
{
    if ((activeScheme() != nullptr) && m_documents.contains(activeScheme()))
        return m_documents[activeScheme()];
    return 0;
}

CScheme* CMainWindow::findScheme(const QString &schemeFileName)
{
    if (schemeFileName.isEmpty())
        return nullptr;

    foreach (CScheme *scheme, m_documents.keys()) {
        if ((scheme != nullptr) && (scheme->fileName() == schemeFileName))
            return scheme;
    }

    return nullptr;
}

void CMainWindow::closeEvent(QCloseEvent *event)
{
    if (saveSchemesBeforeClose(m_documents.keys())) {
        if (isAutoSaveDesktop())
            saveDesktop("desktop.ini");
        if (isAutoSaveConfig())
            saveConfig("config.ini");

        event->accept();
    } else {
        event->ignore();
    }
}

CMainWindow::CMainWindow(QWidget *parent) : QMainWindow(parent)
{
    setObjectName("CMainWindow");

    m_autoSaveConfig = false;
    m_autoSaveDesktop = false;
    m_autoSaveLastScheme = false;
    m_autoLoadLastScheme = false;
    m_gridColor = Qt::black;
    m_gridBkGndColor = Qt::white;
    m_gridStep = 8;
    m_gridPointSize = 0.0;
    m_gridAlign = true;

    m_acNewScheme = nullptr;
    m_acOpenScheme = nullptr;
    m_acSaveScheme = nullptr;
    m_acSaveSchemeAs = nullptr;
    m_acCloseScheme = nullptr;
    m_acQuit = nullptr;
    m_acGrSchemeMouseMode = nullptr;
    m_acCursor = nullptr;
    m_acHand = nullptr;
    m_acLinking = nullptr;
    m_acEditSep = nullptr;
    m_acCopy = nullptr;
    m_acCut = nullptr;
    m_acDelete = nullptr;
    m_acPaste = nullptr;
    m_acDataWindow = nullptr;
    m_acCalcActiveScheme = nullptr;
    m_acCalcAllSchemes = nullptr;
    m_acOptions = nullptr;
    m_acElementOptions = nullptr;

    m_viewToolBarsMenu = nullptr;
    m_viewDocksMenu = nullptr;

    m_schemeEditorContextMenu = nullptr;

    m_algProtoViewDock = nullptr;

    m_workSpaceTabWgt = nullptr;
    m_dataWindow = nullptr;
    m_algorithmProtoMng = nullptr;
    m_algProtoView = nullptr;
    m_schemeCounter = 1;
    m_activeScheme = nullptr;
    m_pThreadEngine = nullptr;

    m_workSpaceTabWgt = new QTabWidget();
    m_workSpaceTabWgt->setObjectName(QStringLiteral("workSpaceTabWgt"));
    m_workSpaceTabWgt->setTabsClosable(true);
    connect(m_workSpaceTabWgt, SIGNAL(currentChanged(int)),
            this, SLOT(onCurrentTabChanged(int)));
    connect(m_workSpaceTabWgt, SIGNAL(tabCloseRequested(int)),
            this, SLOT(onTabCloseRequested(int)));
    setCentralWidget(m_workSpaceTabWgt);

    m_dataWindow = new CDataWindow(this);
    m_dataWindow->setObjectName(QStringLiteral("dataWindow"));
    connect(m_dataWindow, SIGNAL(visibleChanged(bool)),
            this, SLOT(onDataWindowVisibleChanged(bool)));

    m_algorithmProtoMng = new CAlgorithmProtoMng(this);
    m_algorithmProtoMng->setObjectName(QStringLiteral("algorithmProtoMng"));
    m_algorithmProtoMng->addProto(tr("Amp"), tr("General"), &CAmp::staticMetaObject);
    m_algorithmProtoMng->addProto(tr("Sum"), tr("General"), &CSum::staticMetaObject);
    m_algorithmProtoMng->addProto(tr("CSV In"), tr("CSV"), &CCSVIn::staticMetaObject);
    m_algorithmProtoMng->addProto(tr("CSV Out"), tr("CSV"), &CCSVOut::staticMetaObject);
    m_algorithmProtoMng->addProto(tr("A(t)"), tr("TAC"), &CStepExcitation::staticMetaObject);
    m_algorithmProtoMng->addProto(tr("TL"), tr("TAC"), &CTransLink::staticMetaObject);
    connect(m_algorithmProtoMng, SIGNAL(algorithmProtoSelected(CAlgorithmProto*)),
            this, SLOT(onAlgorithmProtoSelected(CAlgorithmProto*)));

    m_algProtoView = new CAlgProtoView(m_algorithmProtoMng, Qt::Horizontal);
    m_algProtoView->setObjectName(QStringLiteral("algProtoView"));
    m_algProtoView->setWindowTitle(tr("Algorithm prototypes"));

    m_pThreadEngine = new CThreadEngine(this);
    m_pThreadEngine->setObjectName(QStringLiteral("threadEngine"));
    connect(m_pThreadEngine, SIGNAL(calcStarted()), m_dataWindow, SLOT(startAutoRefresh()));
    connect(m_pThreadEngine, SIGNAL(calcFinished()), m_dataWindow, SLOT(stopAutoRefresh()));

    QClipboard *clpb = QApplication::clipboard();
    connect(clpb, SIGNAL(changed(QClipboard::Mode)),
            this, SLOT(onClipBoardChanged(QClipboard::Mode)));

    setupActions();
    setupMainMenu();
    setupToolBars();
    setupDocks();
    setupStatusBar();
    setupSchemeEditorContextMenu();

    restoreConfig("config.ini");
    restoreDesktop("desktop.ini");
}

void CMainWindow::setAutoSaveLastScheme(const bool &autoSaveLastScheme)
{
    m_autoSaveLastScheme = autoSaveLastScheme;
}

void CMainWindow::setAutoLoadLastScheme(const bool &autoLoadLastScheme)
{
    m_autoLoadLastScheme = autoLoadLastScheme;
}

void CMainWindow::setGridColor(const QColor &gridColor)
{
    if (m_gridColor == gridColor)
        return;

    m_gridColor = gridColor;

    foreach (CSchemeEditor *schemeEditor, m_documents) {
        if (schemeEditor != nullptr)
            schemeEditor->setGridColor(m_gridColor);
    }
}

void CMainWindow::setGridBkGndColor(const QColor &gridBkGndColor)
{
    if (m_gridBkGndColor == gridBkGndColor)
        return;

    m_gridBkGndColor = gridBkGndColor;

    foreach (CSchemeEditor *schemeEditor, m_documents) {
        if (schemeEditor != nullptr)
            schemeEditor->setGridBkGndColor(m_gridBkGndColor);
    }
}

void CMainWindow::setGridStep(const int &gridStep)
{
    if (m_gridStep == gridStep)
        return;

    m_gridStep = gridStep;

    foreach (CSchemeEditor *schemeEditor, m_documents) {
        if (schemeEditor != nullptr)
            schemeEditor->setGridStep(m_gridStep);
    }
}

void CMainWindow::setGridPointSize(const qreal &gridPointSize)
{
    if (m_gridPointSize == gridPointSize)
        return;

    m_gridPointSize = gridPointSize;

    foreach (CSchemeEditor *schemeEditor, m_documents) {
        if (schemeEditor != nullptr)
            schemeEditor->setGridPointSize(m_gridPointSize);
    }
}

void CMainWindow::setGridAlign(const bool &gridAlign)
{
    if (m_gridAlign == gridAlign)
        return;

    m_gridAlign = gridAlign;

    foreach (CSchemeEditor *schemeEditor, m_documents) {
        if (schemeEditor != nullptr)
            schemeEditor->setGridAlign(m_gridAlign);
    }
}

bool CMainWindow::isDataWindowVisisble(void) const
{
    return (m_dataWindow != nullptr) ? m_dataWindow->isVisible() : false;
}

void CMainWindow::onCursorTriggered(const bool &checked)
{
    if (checked) {
        foreach (CSchemeEditor *schemeEditor, m_documents) {
            if (schemeEditor != nullptr)
                schemeEditor->setMouseMode(CSchemeEditor::MoveSelectMode);
        }
    }
}

void CMainWindow::onHandTriggered(const bool &checked)
{
    if (checked) {
        foreach (CSchemeEditor *schemeEditor, m_documents) {
            if (schemeEditor != nullptr)
                schemeEditor->setMouseMode(CSchemeEditor::MoveSceneMode);
        }
    }
}

void CMainWindow::onLinkingTriggered(const bool &checked)
{
    if (checked) {
        foreach (CSchemeEditor *schemeEditor, m_documents) {
            if (schemeEditor != nullptr)
                schemeEditor->setMouseMode(CSchemeEditor::LinkingMode);
        }
    }
}

void CMainWindow::onAlgorithmProtoSelected(CAlgorithmProto *selectedProto)
{
    if (selectedProto != nullptr) {
        foreach (CSchemeEditor *schemeEditor, m_documents) {
            if (schemeEditor != nullptr)
                schemeEditor->setMouseMode(CSchemeEditor::AddAlgorithmMode);
        }
    }
}

void CMainWindow::onSchemeEditorAddAlgorithmModeFinished(void)
{
    if ((m_algorithmProtoMng != nullptr)
            && (m_algorithmProtoMng->selectedAlgorithmProto() != nullptr)) {
        m_algorithmProtoMng->setSelectedAlgorithmProto(nullptr);
    }

    foreach (CSchemeEditor *schemeEditor, m_documents) {
        if (schemeEditor != nullptr) {
            if ((m_acCursor != nullptr) && m_acCursor->isChecked())
                schemeEditor->setMouseMode(CSchemeEditor::MoveSelectMode);
            else if ((m_acHand != nullptr) && m_acHand->isChecked())
                schemeEditor->setMouseMode(CSchemeEditor::MoveSceneMode);
            else if ((m_acLinking != nullptr) && m_acLinking->isChecked())
                schemeEditor->setMouseMode(CSchemeEditor::LinkingMode);
        }
    }
}

void CMainWindow::onAlgProtosViewDockLocationChanged(const Qt::DockWidgetArea &area)
{
    if (m_algProtoView == nullptr)
        return;

    if ((area == Qt::LeftDockWidgetArea) || (area == Qt::RightDockWidgetArea)) {
        if (m_algProtoViewDock != nullptr) {
            m_algProtoViewDock->setFeatures(m_algProtoViewDock->features()
                                            & ~QDockWidget::DockWidgetVerticalTitleBar);
        }
        m_algProtoView->setOrientation(Qt::Vertical);
    } else if ((area == Qt::TopDockWidgetArea) || (area == Qt::BottomDockWidgetArea)) {
        if (m_algProtoViewDock != nullptr) {
            m_algProtoViewDock->setFeatures(m_algProtoViewDock->features()
                                            | QDockWidget::DockWidgetVerticalTitleBar);
        }
        m_algProtoView->setOrientation(Qt::Horizontal);
    }
}

void CMainWindow::onAlgProtosViewDockTopLevelChanged(const bool &topLevel)
{
    if (topLevel && (m_algProtoView != nullptr))
        m_algProtoView->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
}

void CMainWindow::onSchemeEditorWindowTitleChanged(void)
{
    CSchemeEditor *schemeEditor = qobject_cast<CSchemeEditor*>(sender());
    if ((schemeEditor != nullptr) && (m_workSpaceTabWgt != nullptr)) {
        int tabIndex = m_workSpaceTabWgt->indexOf(schemeEditor);
        if (tabIndex >= 0) {
            m_workSpaceTabWgt->setTabText(tabIndex, schemeEditor->windowTitle());
            if (schemeEditor->scheme() != nullptr)
                m_workSpaceTabWgt->setTabToolTip(tabIndex, schemeEditor->scheme()->fileName());
        }
    }
}

void CMainWindow::onSchemeEditorElementsSelected(const QList<CElement*> &elements)
{
    bool ownSelect = (elements.count() == 1);
    bool hasSelected = !elements.isEmpty();

    if (m_acElementOptions != nullptr) {
        m_acElementOptions->setEnabled(ownSelect);
        m_acElementOptions->setVisible(ownSelect);
    }

    bool enableCopy = hasSelected;
    bool enableDelete = hasSelected;
    if (ownSelect) {
        CElement *element = elements.at(0);
        if (element != nullptr) {
            enableCopy = (element->intercations() & CElement::Copyable);
            enableDelete = (element->intercations() & CElement::Deletable);
        }
    }

    bool enableCut = (enableCopy && enableDelete);
    if (m_acEditSep != nullptr)
        m_acEditSep->setVisible((enableCopy || enableCut || enableDelete));
    if (m_acCopy != nullptr) {
        m_acCopy->setEnabled(enableCopy);
        m_acCopy->setVisible(enableCopy);
    }
    if (m_acCut != nullptr) {
        m_acCut->setEnabled(enableCut);
        m_acCut->setVisible(enableCut);
    }
    if (m_acDelete != nullptr) {
        m_acDelete->setEnabled(enableDelete);
        m_acDelete->setVisible(enableDelete);
    }

    setupSchemeEditorContextMenu();
}

void CMainWindow::onClipBoardChanged(const QClipboard::Mode &mode)
{
    Q_UNUSED(mode)

    QClipboard *clpb = QApplication::clipboard();
    if (clpb->mimeData()->hasHtml()) {
        QDomDocument domDoc;
        QString errMsg;
        int errLine = 0;
        int errCol = 0;
        if (domDoc.setContent(clpb->mimeData()->html(), &errMsg, &errLine, &errCol)) {
            if (activeScheme() && activeScheme()->checkXMLSchemeFormat(domDoc)) {
                if (m_acPaste != nullptr) {
                    m_acPaste->setEnabled(true);
                    m_acPaste->setVisible(true);
                }
            }
        }
    }
}

void CMainWindow::onCurrentTabChanged(const int &currentTabIndex)
{
    m_activeScheme = nullptr;

    if (currentTabIndex >= 0) {
        if (m_workSpaceTabWgt != nullptr) {
            CSchemeEditor *schemeEditor =
                    qobject_cast<CSchemeEditor*>(m_workSpaceTabWgt->currentWidget());
            if (schemeEditor != nullptr)
                m_activeScheme = m_documents.key(schemeEditor, 0);
        }
    }

    bool enableActions = (currentTabIndex >= 0);
    if (m_acSaveScheme != nullptr)
        m_acSaveScheme->setEnabled(enableActions);
    if (m_acSaveSchemeAs != nullptr)
        m_acSaveSchemeAs->setEnabled(enableActions);
    if (m_acCloseScheme != nullptr)
        m_acCloseScheme->setEnabled(enableActions);
    if (m_acCursor != nullptr)
        m_acCursor->setEnabled(enableActions);
    if (m_acHand != nullptr)
        m_acHand->setEnabled(enableActions);
    if (m_acLinking != nullptr)
        m_acLinking->setEnabled(enableActions);
    if (m_acCalcActiveScheme != nullptr)
        m_acCalcActiveScheme->setEnabled(enableActions);
    if (m_acCalcAllSchemes != nullptr)
        m_acCalcAllSchemes->setEnabled(enableActions);

    if (!enableActions) {
        if (m_acEditSep != nullptr)
            m_acEditSep->setVisible(enableActions);
        if (m_acCopy != nullptr) {
            m_acCopy->setEnabled(enableActions);
            m_acCopy->setVisible(enableActions);
        }
        if (m_acCut != nullptr) {
            m_acCut->setEnabled(enableActions);
            m_acCut->setVisible(enableActions);
        }
        if (m_acDelete != nullptr) {
            m_acDelete->setEnabled(enableActions);
            m_acDelete->setVisible(enableActions);
        }
        if (m_acPaste != nullptr) {
            m_acPaste->setEnabled(enableActions);
            m_acPaste->setVisible(enableActions);
        }
        if (m_acElementOptions != nullptr) {
            m_acElementOptions->setEnabled(enableActions);
            m_acElementOptions->setVisible(enableActions);
        }
    }
}

void CMainWindow::onTabCloseRequested(const int &tabIndex)
{
    if (m_workSpaceTabWgt != nullptr) {
        CSchemeEditor *schemeEditor =
                qobject_cast<CSchemeEditor*>(m_workSpaceTabWgt->widget(tabIndex));
        if (schemeEditor != nullptr) {
            CScheme *scheme = m_documents.key(schemeEditor, 0);
            if (scheme != nullptr)
                closeScheme(scheme);
        }
    }
}

void CMainWindow::calcActiveScheme(void)
{
    Q_ASSERT(m_pThreadEngine != nullptr);

    m_pThreadEngine->calcScheme(activeScheme());
}

void CMainWindow::calcAllSchemes()
{
    Q_ASSERT(m_pThreadEngine != nullptr);

    m_pThreadEngine->calcSchemes(m_documents.keys());
}

void CMainWindow::showOptions(void)
{
    COptionsWindow optWnd(this);
    optWnd.setAutoSaveConfig(isAutoSaveConfig());
    optWnd.setAutoSaveDesktop(isAutoSaveDesktop());
    optWnd.setAutoSaveLastScheme(isAutoSaveLastScheme());
    optWnd.setAutoLoadLastScheme(isAutoLoadLastScheme());

    optWnd.setGridColor(gridColor());
    optWnd.setGridBkGndColor(gridBkGndColor());
    optWnd.setGridStep(gridStep());
    optWnd.setGridPointSize(gridPointSize());
    optWnd.setGridAlign(isGridAlign());

    if (activeScheme() != nullptr) {
        optWnd.setSchemeWidhet(activeScheme()->sceneRect().width());
        optWnd.setSchemeHeight(activeScheme()->sceneRect().height());
    }
    if (optWnd.schemeTab() != nullptr)
        optWnd.schemeTab()->setEnabled((activeScheme()));

//    if ((m_engine != nullptr) && (m_engine->framer() != nullptr)) {
//        optWnd.setStartTime(m_engine->framer()->startTime());
//        optWnd.setTimeStep(m_engine->framer()->timeStep());
//        optWnd.setEndTime(m_engine->framer()->endTime());
//    }
//    if (optWnd.calcParamTab() != nullptr) {
//        optWnd.calcParamTab()->setEnabled(((m_engine != nullptr)
//                                           && (m_engine->framer() != nullptr)));
//    }

    int dlgResult = optWnd.exec();
    switch (dlgResult) {
    case QDialog::Accepted:
        setAutoSaveConfig(optWnd.isAutoSaveConfig());
        setAutoSaveDesktop(optWnd.isAutoSaveDesktop());
        setAutoSaveLastScheme(optWnd.isAutoSaveLastScheme());
        setAutoLoadLastScheme(optWnd.isAutoLoadLastScheme());

        setGridColor(optWnd.gridColor());
        setGridBkGndColor(optWnd.gridBkGndColor());
        setGridStep(optWnd.gridStep());
        setGridPointSize(optWnd.gridPointSize());
        setGridAlign(optWnd.isGridAlign());

        if (activeScheme() != nullptr)
            activeScheme()->setSceneRect(0.0, 0.0, optWnd.schemeWidth(), optWnd.schemeHeight());

//        if ((m_engine != nullptr) && (m_engine->framer() != nullptr)) {
//            m_engine->framer()->setStartTime(optWnd.startTime());
//            m_engine->framer()->setTimeStep(optWnd.timeStep());
//            m_engine->framer()->setEndTime(optWnd.endTime());
//        }
        break;
    case QDialog::Rejected:break;
    }
}

void CMainWindow::setDataWindowVisible(const bool &visible)
{
    if (m_dataWindow != nullptr)
        m_dataWindow->setVisible(visible);
}

void CMainWindow::onDataWindowVisibleChanged(const bool &visible)
{
    if ((m_acDataWindow != nullptr) && (m_acDataWindow->isChecked() != visible))
        m_acDataWindow->setChecked(visible);
}

CScheme* CMainWindow::newScheme(CScheme *scheme)
{
    CSchemeEditor *schemeEditor = new CSchemeEditor(this);
    schemeEditor->setObjectName(QStringLiteral("schemeEditor"));
    schemeEditor->setupGrid(gridColor(), gridBkGndColor(),
                            gridStep(), gridPointSize(), isGridAlign());
    if (m_schemeEditorContextMenu != nullptr)
        schemeEditor->setContextMenu(m_schemeEditorContextMenu);
    connect(schemeEditor, SIGNAL(addAlgorithmModeFinished()),
            this, SLOT(onSchemeEditorAddAlgorithmModeFinished()));
    connect(schemeEditor, SIGNAL(windowTitleChanged()),
            this, SLOT(onSchemeEditorWindowTitleChanged()));
    connect(schemeEditor, SIGNAL(elementsSelected(QList<CElement*>)),
            this, SLOT(onSchemeEditorElementsSelected(QList<CElement*>)));

    if (scheme == nullptr) {
        scheme = new CScheme(m_schemeCounter++, this);
        scheme->setObjectName(QStringLiteral("scheme"));
        scheme->setSceneRect(0.0, 0.0, 2000.0, 2000.0);
        scheme->setNewScheme(true);
        scheme->setAlgorithmProtoMng(m_algorithmProtoMng);
    }
    schemeEditor->setScheme(scheme);
    connect(scheme, SIGNAL(destroyed(QObject*)), this, SLOT(onSchemeDestroyed(QObject*)));

    m_documents[scheme] = schemeEditor;
    if (m_workSpaceTabWgt != nullptr) {
        m_workSpaceTabWgt->addTab(schemeEditor, schemeEditor->windowTitle());
        m_workSpaceTabWgt->setCurrentWidget(schemeEditor);
    }
    if (m_dataWindow != nullptr)
        m_dataWindow->addScheme(scheme);

    return scheme;
}

void CMainWindow::saveScheme(CScheme *scheme)
{
    CScheme *savedScheme = scheme;
    if (savedScheme == nullptr)
        savedScheme = activeScheme();
    if (savedScheme == nullptr)
        return;

    if (savedScheme->isNewScheme()) {
        if (saveSchemeAs(savedScheme))
            savedScheme->setNewScheme(false);
    } else {
        writeScheme(savedScheme, savedScheme->fileName());
    }
}

bool CMainWindow::saveSchemeAs(CScheme *scheme)
{
    CScheme *savedScheme = scheme;
    if (savedScheme == nullptr)
        savedScheme = activeScheme();
    if (savedScheme == nullptr)
        return false;

    QString flt = tr("gamma cell V0 schemes (*.scm)");
    QString selFlt = flt;
    QString fileName = QFileDialog::getSaveFileName(this, "Save as...", savedScheme->fileName(),
                                                    flt, &selFlt);
    if (fileName.isEmpty())
        return false;

    savedScheme->setFileName(fileName);
    writeScheme(savedScheme, savedScheme->fileName());
    return true;
}

bool CMainWindow::openSchemes(const QStringList &fileNames)
{
    QStringList schemeNames = fileNames;

    if (schemeNames.isEmpty()) {
        QString flt = tr("gamma cell V0 schemes (*.scm)");
        QString selFlt = flt;
        schemeNames = QFileDialog::getOpenFileNames(this, "Open", QString(), flt, &selFlt);
    }
    if (schemeNames.isEmpty())
        return false;

    bool openResult = false;
    int openedSchemeCounter = m_schemeCounter + 1;
    foreach (QString schemeName, schemeNames) {
        if (schemeName.isEmpty() || !QFile::exists(schemeName))
            continue;

        CScheme *scheme = findScheme(schemeName);
        if (scheme != nullptr) {
            if ((m_workSpaceTabWgt != nullptr)
                    && (m_documents.contains(scheme) && (m_documents[scheme] != nullptr))) {
                m_workSpaceTabWgt->setCurrentWidget(m_documents[scheme]);
            }
        } else {
            scheme = new CScheme(openedSchemeCounter, this);
            scheme->setObjectName(QStringLiteral("scheme"));
            scheme->setAlgorithmProtoMng(m_algorithmProtoMng);
            if (readScheme(scheme, schemeName)) {
                newScheme(scheme);
                scheme->setFileName(schemeName);
                ++openedSchemeCounter;
            } else {
                scheme->deleteLater();
                continue;
            }
        }
        openResult = true;
    }

    return openResult;
}

bool CMainWindow::openScheme(const QString &fileName)
{
    return openSchemes(fileName.isEmpty() ? QStringList() : QStringList() << fileName);
}

bool CMainWindow::closeScheme(CScheme *scheme)
{
    CScheme *closedScheme = scheme;
    if (closedScheme == nullptr)
        closedScheme = activeScheme();
    if (closedScheme == nullptr)
        return false;

    if (closedScheme != nullptr) {
        if (!saveSchemesBeforeClose(QList<CScheme*>() << closedScheme))
            return false;
        closedScheme->deleteLater();
    }

    return true;
}

void CMainWindow::onSchemeDestroyed(QObject *objScheme)
{
    CScheme *scheme = (CScheme*)objScheme;
    if (m_documents.contains(scheme)) {
        if (m_documents[scheme] != nullptr)
            m_documents[scheme]->deleteLater();
        m_documents.remove(scheme);
    }
}

void CMainWindow::copy(void)
{
    CSchemeEditor *schemeEditor = activeSchemeEditor();
    if (schemeEditor != nullptr)
        schemeEditor->copySelected();
}

void CMainWindow::cut(void)
{
    CSchemeEditor *schemeEditor = activeSchemeEditor();
    if (schemeEditor != nullptr)
        schemeEditor->cutSelected();
}

void CMainWindow::del(void)
{
    CSchemeEditor *schemeEditor = activeSchemeEditor();
    if (schemeEditor != nullptr)
        schemeEditor->deleteSelected();
}

void CMainWindow::paste(void)
{
    CSchemeEditor *schemeEditor = activeSchemeEditor();
    if (schemeEditor != nullptr)
        schemeEditor->pasteSelected();
}

//Must be refact
void CMainWindow::elementOptions(void)
{
    CSchemeEditor *schemeEditor = activeSchemeEditor();
    if (schemeEditor != nullptr)
        schemeEditor->showElementOptions();
}

void CMainWindow::saveDesktop(const QString &fileName)
{
    if (fileName.isEmpty())
        return;

    QSettings desk(fileName, QSettings::IniFormat);
    desk.setValue("MainWindow/geometry", saveGeometry());
    desk.setValue("MainWindow/state", saveState());

    if (m_dataWindow != nullptr) {
        desk.setValue("DataWindow/geometry", m_dataWindow->saveGeometry());
        desk.setValue("DataWindow/state", m_dataWindow->saveState());
        desk.setValue("DataWindow/visible", isDataWindowVisisble());
        if (m_dataWindow->splitter() != nullptr)
            desk.setValue("DataWindow/splitter", m_dataWindow->splitter()->saveState());
    }

    QStringList oldSchemeFileList;
    foreach (CScheme *scheme, m_documents.keys()) {
        if ((scheme != nullptr) && !scheme->isNewScheme() && !scheme->fileName().isEmpty())
            oldSchemeFileList << scheme->fileName();
    }
    desk.setValue("MainWindow/Old schemes", oldSchemeFileList);
}

void CMainWindow::restoreDesktop(const QString &fileName)
{
    if (fileName.isEmpty())
        return;

    QSettings desk(fileName, QSettings::IniFormat);
    bool openResult = false;
    if (isAutoLoadLastScheme()) {
        QStringList oldSchemeFileList =
                desk.value("MainWindow/Old schemes", QString()).toStringList();
        if (!oldSchemeFileList.isEmpty())
            openResult = openSchemes(oldSchemeFileList);
    }
    if (!openResult)
        newScheme();

    if (m_dataWindow != nullptr) {
        m_dataWindow->restoreGeometry(desk.value("DataWindow/geometry").toByteArray());
        m_dataWindow->restoreState(desk.value("DataWindow/state").toByteArray());
        setDataWindowVisible(desk.value("DataWindow/visible").toBool());
        if (m_dataWindow->splitter() != nullptr)
            m_dataWindow->splitter()->restoreState(desk.value("DataWindow/splitter").toByteArray());
    }

    restoreGeometry(desk.value("MainWindow/geometry").toByteArray());
    restoreState(desk.value("MainWindow/state").toByteArray());
}

void CMainWindow::saveConfig(const QString &fileName)
{
    if (fileName.isEmpty())
        return;

    QSettings cfg(fileName, QSettings::IniFormat);
    cfg.setValue("General/autoSaveConfig", isAutoSaveConfig());
    cfg.setValue("General/autoSaveDesktop", isAutoSaveDesktop());
    cfg.setValue("General/autoSaveLastScheme", isAutoSaveLastScheme());
    cfg.setValue("General/autoLoadLastScheme", isAutoLoadLastScheme());

    cfg.setValue("SchemeEditor/gridColor", gridColor());
    cfg.setValue("SchemeEditor/gridBkGndColor", gridBkGndColor());
    cfg.setValue("SchemeEditor/gridStep", gridStep());
    cfg.setValue("SchemeEditor/gridPointSize", gridPointSize());
    cfg.setValue("SchemeEditor/gridAlign", isGridAlign());

    if (m_dataWindow != nullptr) {
        cfg.setValue("DataWindow/autoRefresh", m_dataWindow->isAutoRefreshEnabled());
        cfg.setValue("DataWindow/autoRefreshInterval", m_dataWindow->autoRefreshInterval());
    }

//    if ((m_engine != nullptr) && (m_engine->framer() != nullptr)) {
//        cfg.setValue("Engine/startTime", m_engine->framer()->startTime());
//        cfg.setValue("Engine/timeStep", m_engine->framer()->timeStep());
//        cfg.setValue("Engine/endTime", m_engine->framer()->endTime());
//    }
}

void CMainWindow::restoreConfig(const QString &fileName)
{
    if (fileName.isEmpty())
        return;

    QSettings cfg(fileName, QSettings::IniFormat);
    setAutoSaveConfig(cfg.value("General/autoSaveConfig", false).toBool());
    setAutoSaveDesktop(cfg.value("General/autoSaveDesktop", false).toBool());
    setAutoSaveLastScheme(cfg.value("General/autoSaveLastScheme", false).toBool());
    setAutoLoadLastScheme(cfg.value("General/autoLoadLastScheme", false).toBool());

    setGridColor(cfg.value("SchemeEditor/gridColor", m_gridColor).value<QColor>());
    setGridBkGndColor(cfg.value("SchemeEditor/gridBkGndColor", m_gridBkGndColor).value<QColor>());
    setGridStep(cfg.value("SchemeEditor/gridStep", m_gridStep).toInt());
    setGridPointSize(cfg.value("SchemeEditor/gridPointSize", m_gridPointSize).toDouble());
    setGridAlign(cfg.value("SchemeEditor/gridAlign", m_gridAlign).toBool());

    if (m_dataWindow != nullptr) {
        m_dataWindow->setAutoRefresh(cfg.value("DataWindow/autoRefresh", false).toBool());

        int autoRefreshInterval = cfg.value("DataWindow/autoRefreshInterval", 5).toInt();
        m_dataWindow->setAutoRefreshInterval(autoRefreshInterval);
    }

//    if ((m_engine != nullptr) && (m_engine->framer() != nullptr)) {
//            m_engine->framer()->setStartTime(cfg.value("Engine/startTime", 0.0).toDouble());
//            m_engine->framer()->setTimeStep(cfg.value("Engine/timeStep", 0.01).toDouble());
//            m_engine->framer()->setEndTime(cfg.value("Engine/endTime", 10.0).toDouble());
//    }
}
