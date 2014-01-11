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
#include "../scheme/algorithmproto/calgorithmproto.h"
#include "../scheme/algorithmproto/calgorithmprotomng.h"
#include "calgprotoview.h"
#include "../scheme/cscheme.h"
#include "../scheme/cschemeeditor.h"
#include "../engine/cengine.h"
#include "../engine/ctimeframegenerator.h"
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
	connect(m_acQuit, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));

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

	//Calc
	m_acCalc = new QAction(tr("Calc"), this);
	m_acCalc->setObjectName(QStringLiteral("acCalc"));
	m_acCalc->setEnabled(false);
	connect(m_acCalc, SIGNAL(triggered()), this, SLOT(calc()));

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
	if(m_acNewScheme) fileMenu->addAction(m_acNewScheme);
	if(m_acOpenScheme)
	{
		fileMenu->addAction(m_acOpenScheme);
		fileMenu->addSeparator();
	}
	if(m_acSaveScheme) fileMenu->addAction(m_acSaveScheme);
	if(m_acSaveSchemeAs)
	{
		fileMenu->addAction(m_acSaveSchemeAs);
		fileMenu->addSeparator();
	}
	if(m_acCloseScheme)
	{
		fileMenu->addAction(m_acCloseScheme);
		fileMenu->addSeparator();
	}
	if(m_acQuit) fileMenu->addAction(m_acQuit);

	QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
	if(m_acGrSchemeMouseMode) editMenu->addActions(m_acGrSchemeMouseMode->actions());
	if(m_acEditSep) editMenu->addAction(m_acEditSep);
	if(m_acCopy) editMenu->addAction(m_acCopy);
	if(m_acCut) editMenu->addAction(m_acCut);
	if(m_acDelete) editMenu->addAction(m_acDelete);
	if(m_acPaste) editMenu->addAction(m_acPaste);

	QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
	if(m_acDataWindow) viewMenu->addAction(m_acDataWindow);
	m_viewToolBarsMenu = viewMenu->addMenu(tr("Tool bars"));
	m_viewToolBarsMenu->setObjectName("viewToolBarsMenu");
	m_viewDocksMenu = viewMenu->addMenu(tr("Docks"));
	m_viewDocksMenu->setObjectName(QStringLiteral("viewDocksMenu"));

	QMenu *calcMenu = menuBar()->addMenu(tr("&Calculate"));
	if(m_acCalc) calcMenu->addAction(m_acCalc);

//	menuBar()->addMenu(tr("&Algorithms"));

	QMenu *optionsMenu = menuBar()->addMenu(tr("&Options"));
	if(m_acOptions) optionsMenu->addAction(m_acOptions);
	if(m_acElementOptions) optionsMenu->addAction(m_acElementOptions);

	menuBar()->addMenu(tr("&Help"));	
}

void CMainWindow::setupToolBars(void)
{
	//File
	QToolBar *tbFile = addToolBar(tr("File"));
	tbFile->setObjectName(QStringLiteral("tbMain"));
	if(m_viewToolBarsMenu) m_viewToolBarsMenu->addAction(tbFile->toggleViewAction());
	if(m_acNewScheme) tbFile->addAction(m_acNewScheme);
	if(m_acOpenScheme)
	{
		tbFile->addAction(m_acOpenScheme);
		tbFile->addSeparator();
	}
	if(m_acSaveScheme) tbFile->addAction(m_acSaveScheme);
	if(m_acSaveSchemeAs)
	{
		tbFile->addAction(m_acSaveSchemeAs);
		tbFile->addSeparator();
	}
	if(m_acCloseScheme)
	{
		tbFile->addAction(m_acCloseScheme);
		tbFile->addSeparator();
	}
	if(m_acQuit) tbFile->addAction(m_acQuit);

	//Edit
	addToolBarBreak();
	QToolBar *tbEdit = addToolBar(tr("Edit"));
	tbEdit->setObjectName(QStringLiteral("tbEdit"));
	if(m_viewToolBarsMenu) m_viewToolBarsMenu->addAction(tbEdit->toggleViewAction());
	if(m_acGrSchemeMouseMode) tbEdit->addActions(m_acGrSchemeMouseMode->actions());
	if(m_acEditSep) tbEdit->addAction(m_acEditSep);
	if(m_acCopy) tbEdit->addAction(m_acCopy);
	if(m_acCut) tbEdit->addAction(m_acCut);
	if(m_acDelete) tbEdit->addAction(m_acDelete);

	//View
	QToolBar *tbView = addToolBar(tr("View"));
	tbView->setObjectName(QStringLiteral("tbView"));
	if(m_viewToolBarsMenu) m_viewToolBarsMenu->addAction(tbView->toggleViewAction());
	if(m_acDataWindow) tbView->addAction(m_acDataWindow);

	//Calc
	QToolBar *tbCalc = addToolBar(tr("Calculation"));
	tbCalc->setObjectName(QStringLiteral("tbCalc"));
	if(m_viewToolBarsMenu) m_viewToolBarsMenu->addAction(tbCalc->toggleViewAction());
	if(m_acCalc) tbCalc->addAction(m_acCalc);

	//Options
	QToolBar *tbOptions = addToolBar(tr("Options"));
	tbOptions->setObjectName(QStringLiteral("tbOptions"));
	if(m_viewToolBarsMenu) m_viewToolBarsMenu->addAction(tbOptions->toggleViewAction());
	if(m_acOptions) tbOptions->addAction(m_acOptions);
}

void CMainWindow::setupDocks(void)
{
	if(m_algProtoView)
	{
		m_algProtoViewDock = new QDockWidget(m_algProtoView->windowTitle());
		m_algProtoViewDock->setObjectName(QStringLiteral("algProtoViewDock"));
		m_algProtoViewDock->setAllowedAreas(Qt::AllDockWidgetAreas);
		m_algProtoViewDock->setWidget(m_algProtoView);
		m_algProtoViewDock->setTitleBarWidget(new QLabel(""));
		if(m_viewDocksMenu) m_viewDocksMenu->addAction(m_algProtoViewDock->toggleViewAction());
		connect(m_algProtoViewDock, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT(onAlgProtosViewDockLocationChanged(Qt::DockWidgetArea)));
		connect(m_algProtoViewDock, SIGNAL(topLevelChanged(bool)), this, SLOT(onAlgProtosViewDockTopLevelChanged(bool)));
		addDockWidget(Qt::TopDockWidgetArea, m_algProtoViewDock);
	}
}

void CMainWindow::setupStatusBar(void)
{
	statusBar()->showMessage(tr("Ready"));
}

void CMainWindow::setupSchemeEditorContextMenu(void)
{
	if(!m_schemeEditorContextMenu)
	{
		m_schemeEditorContextMenu = new QMenu();
		m_schemeEditorContextMenu->setObjectName(QStringLiteral("schemeEditorContextMenu"));
	}

	m_schemeEditorContextMenu->clear();

	if(m_acElementOptions) m_schemeEditorContextMenu->addAction(m_acElementOptions);

	if(m_schemeEditor && (m_schemeEditor->selectedElements().count() == 1))
	{
		if(!m_schemeEditor->selectedElements().at(0)->actions().isEmpty())
		{
			if(!m_schemeEditorContextMenu->isEmpty()) m_schemeEditorContextMenu->addSeparator();
			m_schemeEditorContextMenu->addActions(m_schemeEditor->selectedElements().at(0)->actions());
		}
	}
	else
	{
		if(m_acCursor)
		{
			if(!m_schemeEditorContextMenu->isEmpty()) m_schemeEditorContextMenu->addSeparator();
			m_schemeEditorContextMenu->addAction(m_acCursor);
		}
		if(m_acHand) m_schemeEditorContextMenu->addAction(m_acHand);
		if(m_acLinking) m_schemeEditorContextMenu->addAction(m_acLinking);
	}
	if(m_acEditSep) m_schemeEditorContextMenu->addAction(m_acEditSep);
	if(m_acCopy) m_schemeEditorContextMenu->addAction(m_acCopy);
	if(m_acCut) m_schemeEditorContextMenu->addAction(m_acCut);
	if(m_acDelete) m_schemeEditorContextMenu->addAction(m_acDelete);
}

void CMainWindow::writeScheme(CScheme *scheme, const QString &fileName)
{
	if(!scheme || fileName.isEmpty()) return;

	QFile fileHandler(fileName);
	if(!fileHandler.open(QIODevice::WriteOnly)) return;

	stSchemeDesc schemeDesc;
	schemeDesc.width = scheme->sceneRect().width();
	schemeDesc.height = scheme->sceneRect().height();

	QTextStream(&fileHandler) << scheme->toXMLDom(scheme->elements(), &schemeDesc).toString();
	fileHandler.close();

	scheme->setNewScheme(false);
	scheme->setModified(false);
}

void CMainWindow::readScheme(CScheme *scheme, const QString &fileName)
{
	if(!scheme || fileName.isEmpty()) return;

	QFile fileHandler(fileName);
	if(!fileHandler.exists()) return;

	QDomDocument domDoc;
	QString errMsg;
	int errLine = 0;
	int errCol = 0;
	if(!domDoc.setContent(&fileHandler, &errMsg, &errLine, &errCol)) return;

	stSchemeDesc schemeDesc;
	QList<CElement*> elements = scheme->fromXMLDom(domDoc, &schemeDesc);
	scheme->setSceneRect(0.0, 0.0, schemeDesc.width, schemeDesc.height);
	scheme->addElements(elements);

	scheme->setNewScheme(false);
	scheme->setModified(false);
}

bool CMainWindow::saveBeforeClose(void)
{
	QList<CScheme*> modSchemes;

	if(m_scheme && m_scheme->isModified()) modSchemes << m_scheme;

	if(!modSchemes.isEmpty())
	{
		CSaveModSchemesDlg saveModDlg;

		saveModDlg.setSavedSchemes(modSchemes);
		int execResult = saveModDlg.exec();
		switch(execResult)
		{
			case QDialog::Accepted:
			{
				QList<stSavedScheme> savedSchemes = saveModDlg.savedSchemes();
				foreach(stSavedScheme savedScheme, savedSchemes)
				{
					if(!savedScheme.scheme || !savedScheme.canBeStored) continue;

					saveScheme();//savedScheme.scheme);
				}

				return true;
			}
			case QDialog::Rejected: return false;
			case CSaveModSchemesDlg::Discarded: return true;
		}
	}

	return true;
}

void CMainWindow::closeEvent(QCloseEvent *event)
{
	if(saveBeforeClose())
	{
		if(isAutoSaveDesktop()) saveDesktop("desktop.ini");
		if(isAutoSaveConfig()) saveConfig("config.ini");

		event->accept();
	}
	else
	{
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

	m_acNewScheme = 0;
	m_acOpenScheme = 0;
	m_acSaveScheme = 0;
	m_acSaveSchemeAs = 0;
	m_acCloseScheme = 0;
	m_acQuit = 0;
	m_acGrSchemeMouseMode = 0;
	m_acCursor = 0;
	m_acHand = 0;
	m_acLinking = 0;
	m_acEditSep = 0;
	m_acCopy = 0;
	m_acCut = 0;
	m_acDelete = 0;
	m_acPaste = 0;
	m_acDataWindow = 0;
	m_acCalc = 0;
	m_acOptions = 0;
	m_acElementOptions = 0;

	m_viewToolBarsMenu = 0;
	m_viewDocksMenu = 0;

	m_schemeEditorContextMenu = 0;

	m_algProtoViewDock = 0;

	m_workSpaceTabWgt = 0;
	m_dataWindow = 0;
	m_algorithmProtoMng = 0;
	m_algProtoView = 0;
	m_schemeEditor = 0;
	m_scheme = 0;
	m_engine = 0;

	m_workSpaceTabWgt = new QTabWidget();
	m_workSpaceTabWgt->setObjectName(QStringLiteral("workSpaceTabWgt"));
	setCentralWidget(m_workSpaceTabWgt);

	m_dataWindow = new CDataWindow(this);
	m_dataWindow->setObjectName(QStringLiteral("dataWindow"));

	m_algorithmProtoMng = new CAlgorithmProtoMng(this);
	m_algorithmProtoMng->setObjectName(QStringLiteral("algorithmProtoMng"));
	m_algorithmProtoMng->addProto(tr("Amp"), tr("General"), &CAmp::staticMetaObject);
	m_algorithmProtoMng->addProto(tr("Sum"), tr("General"), &CSum::staticMetaObject);
	m_algorithmProtoMng->addProto(tr("CSV In"), tr("CSV"), &CCSVIn::staticMetaObject);
	m_algorithmProtoMng->addProto(tr("CSV Out"), tr("CSV"), &CCSVOut::staticMetaObject);
	m_algorithmProtoMng->addProto(tr("A(t)"), tr("TAC"), &CStepExcitation::staticMetaObject);
	m_algorithmProtoMng->addProto(tr("TL"), tr("TAC"), &CTransLink::staticMetaObject);
	connect(m_algorithmProtoMng, SIGNAL(algorithmProtoSelected(CAlgorithmProto*)), this, SLOT(onAlgorithmProtoSelected(CAlgorithmProto*)));

	m_algProtoView = new CAlgProtoView(m_algorithmProtoMng, Qt::Horizontal);
	m_algProtoView->setObjectName(QStringLiteral("algProtoView"));
	m_algProtoView->setWindowTitle(tr("Algorithm prototypes"));

	m_engine = new CEngine(this);
	m_engine->setObjectName(QStringLiteral("engine"));
	connect(m_engine, SIGNAL(calcStarted()), m_dataWindow, SLOT(startAutoRefresh()));
	connect(m_engine, SIGNAL(calcStopped()), m_dataWindow, SLOT(stopAutoRefresh()));

	QClipboard *clpb = QApplication::clipboard();
	connect(clpb, SIGNAL(changed(QClipboard::Mode)), this, SLOT(onClipBoardChanged(QClipboard::Mode)));

	setupActions();
	setupMainMenu();
	setupToolBars();
	setupDocks();
	setupStatusBar();
	setupSchemeEditorContextMenu();

	restoreConfig("config.ini");
	restoreDesktop("desktop.ini");
}

void CMainWindow::setGridColor(const QColor &gridColor)
{
	if(m_gridColor == gridColor) return;

	m_gridColor = gridColor;
	if(m_schemeEditor) m_schemeEditor->setGridColor(m_gridColor);
}

void CMainWindow::setGridBkGndColor(const QColor &gridBkGndColor)
{
	if(m_gridBkGndColor == gridBkGndColor) return;

	m_gridBkGndColor = gridBkGndColor;
	if(m_schemeEditor) m_schemeEditor->setGridBkGndColor(m_gridBkGndColor);
}

void CMainWindow::setGridStep(const int &gridStep)
{
	if(m_gridStep == gridStep) return;

	m_gridStep = gridStep;
	if(m_schemeEditor) m_schemeEditor->setGridStep(m_gridStep);
}

void CMainWindow::setGridPointSize(const qreal &gridPointSize)
{
	if(m_gridPointSize == gridPointSize) return;

	m_gridPointSize = gridPointSize;
	if(m_schemeEditor) m_schemeEditor->setGridPointSize(gridPointSize);
}

void CMainWindow::setGridAlign(const bool &gridAlign)
{
	if(m_gridAlign == gridAlign) return;

	m_gridAlign = gridAlign;
	if(m_schemeEditor) m_schemeEditor->setGridAlign(m_gridAlign);
}

bool CMainWindow::isDataWindowVisisble(void) const
{
	return m_dataWindow ? m_dataWindow->isVisible() : false;
}

void CMainWindow::onCursorTriggered(const bool &checked)
{
	if(checked && m_schemeEditor) m_schemeEditor->setMouseMode(CSchemeEditor::MoveSelectMode);
}

void CMainWindow::onHandTriggered(const bool &checked)
{
	if(checked && m_schemeEditor) m_schemeEditor->setMouseMode(CSchemeEditor::MoveSceneMode);
}

void CMainWindow::onLinkingTriggered(const bool &checked)
{
	if(checked && m_schemeEditor) m_schemeEditor->setMouseMode(CSchemeEditor::LinkingMode);
}

void CMainWindow::onAlgorithmProtoSelected(CAlgorithmProto *selectedProto)
{
	if(m_schemeEditor)
	{
		if(selectedProto) m_schemeEditor->setMouseMode(CSchemeEditor::AddAlgorithmMode);
	}
}

void CMainWindow::onSchemeEditorAddAlgorithmModeFinished(void)
{
	if(m_algorithmProtoMng && m_algorithmProtoMng->selectedAlgorithmProto()) m_algorithmProtoMng->setSelectedAlgorithmProto(0);
	if(m_schemeEditor)
	{
		if(m_acCursor && m_acCursor->isChecked())
		{
			m_schemeEditor->setMouseMode(CSchemeEditor::MoveSelectMode);
		}
		else if(m_acHand && m_acHand->isChecked())
		{
			m_schemeEditor->setMouseMode(CSchemeEditor::MoveSceneMode);
		}
		else if(m_acLinking && m_acLinking->isChecked())
		{
			m_schemeEditor->setMouseMode(CSchemeEditor::LinkingMode);
		}
	}
}

void CMainWindow::onAlgProtosViewDockLocationChanged(const Qt::DockWidgetArea &area)
{
	if(!m_algProtoView) return;
	if(area == Qt::LeftDockWidgetArea || area == Qt::RightDockWidgetArea)
	{
		if(m_algProtoViewDock) m_algProtoViewDock->setFeatures(m_algProtoViewDock->features() & ~QDockWidget::DockWidgetVerticalTitleBar);
		m_algProtoView->setOrientation(Qt::Vertical);
	}
	else if(area == Qt::TopDockWidgetArea || area == Qt::BottomDockWidgetArea)
	{
		if(m_algProtoViewDock) m_algProtoViewDock->setFeatures(m_algProtoViewDock->features() | QDockWidget::DockWidgetVerticalTitleBar);
		m_algProtoView->setOrientation(Qt::Horizontal);
	}
}

void CMainWindow::onAlgProtosViewDockTopLevelChanged(const bool &topLevel)
{
	if(topLevel && m_algProtoView) m_algProtoView->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
}

void CMainWindow::onSchemeEditorWindowTitleChanged(void)
{
	if(m_schemeEditor && m_workSpaceTabWgt)
	{
		int tabIndex = m_workSpaceTabWgt->indexOf(m_schemeEditor);
		if(tabIndex >= 0) m_workSpaceTabWgt->setTabText(tabIndex, m_schemeEditor->windowTitle());
	}
}

void CMainWindow::onSchemeEditorElementsSelected(const QList<CElement*> &elements)
{
	bool ownSelect = (elements.count() == 1);
	bool hasSelected = !elements.isEmpty();

	if(m_acElementOptions)
	{
		m_acElementOptions->setEnabled(ownSelect);
		m_acElementOptions->setVisible(ownSelect);
	}
	bool enableCopy = hasSelected;
	bool enableDelete = hasSelected;
	if(ownSelect)
	{
		CElement *element = elements.at(0);
		if(element)
		{
			enableCopy = (element->intercations() & CElement::Copyable);
			enableDelete = (element->intercations() & CElement::Deletable);
		}
	}
	bool enableCut = (enableCopy && enableDelete);
	if(m_acEditSep) m_acEditSep->setVisible((enableCopy || enableCut || enableDelete));
	if(m_acCopy)
	{
		m_acCopy->setEnabled(enableCopy);
		m_acCopy->setVisible(enableCopy);
	}
	if(m_acCut)
	{
		m_acCut->setEnabled(enableCut);
		m_acCut->setVisible(enableCut);
	}
	if(m_acDelete)
	{
		m_acDelete->setEnabled(enableDelete);
		m_acDelete->setVisible(enableDelete);
	}

	setupSchemeEditorContextMenu();
}

void CMainWindow::onClipBoardChanged(const QClipboard::Mode &mode)
{
	Q_UNUSED(mode)

	QClipboard *clpb = QApplication::clipboard();
	if(clpb->mimeData()->hasHtml())
	{
		QDomDocument domDoc;
		QString errMsg;
		int errLine = 0;
		int errCol = 0;
		if(domDoc.setContent(clpb->mimeData()->html(), &errMsg, &errLine, &errCol))
		{
			if(m_scheme && m_scheme->checkXMLSchemeFormat(domDoc))
			{
				if(m_acPaste)
				{
					m_acPaste->setEnabled(true);
					m_acPaste->setVisible(true);
				}
			}
		}
	}
}

void CMainWindow::calc(void)
{
	if(m_engine) m_engine->calc();
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

	if(m_scheme)
	{
		optWnd.setSchemeWidhet(m_scheme->sceneRect().width());
		optWnd.setSchemeHeight(m_scheme->sceneRect().height());
	}
	if(optWnd.schemeTab()) optWnd.schemeTab()->setEnabled((m_scheme));

	if(m_engine && m_engine->framer())
	{
		optWnd.setStartTime(m_engine->framer()->startTime());
		optWnd.setTimeStep(m_engine->framer()->timeStep());
		optWnd.setEndTime(m_engine->framer()->endTime());
	}
	if(optWnd.calcParamTab()) optWnd.calcParamTab()->setEnabled((m_engine && m_engine->framer()));

	int dlgResult = optWnd.exec();
	switch(dlgResult)
	{
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

			if(m_scheme)
			{
				m_scheme->setSceneRect(0.0, 0.0, optWnd.schemeWidth(), optWnd.schemeHeight());
			}

			if(m_engine && m_engine->framer())
			{
				m_engine->framer()->setStartTime(optWnd.startTime());
				m_engine->framer()->setTimeStep(optWnd.timeStep());
				m_engine->framer()->setEndTime(optWnd.endTime());
			}
		break;
		case QDialog::Rejected:break;
	}
}

void CMainWindow::setDataWindowVisible(const bool &visible)
{
	if(m_dataWindow) m_dataWindow->setVisible(visible);

	if(m_acDataWindow && (sender() != m_acDataWindow))
	{
		m_acDataWindow->setChecked(visible);
	}
}

void CMainWindow::newScheme(void)
{
	if(m_scheme) closeScheme();

	m_schemeEditor = new CSchemeEditor(this);
	m_schemeEditor->setObjectName(QStringLiteral("schemeEditor"));
	m_schemeEditor->setupGrid(gridColor(), gridBkGndColor(), gridStep(), gridPointSize(), isGridAlign());
	if(m_schemeEditorContextMenu) m_schemeEditor->setContextMenu(m_schemeEditorContextMenu);
	connect(m_schemeEditor, SIGNAL(addAlgorithmModeFinished()), this, SLOT(onSchemeEditorAddAlgorithmModeFinished()));
	connect(m_schemeEditor, SIGNAL(windowTitleChanged()), this, SLOT(onSchemeEditorWindowTitleChanged()));
	connect(m_schemeEditor, SIGNAL(elementsSelected(QList<CElement*>)), this, SLOT(onSchemeEditorElementsSelected(QList<CElement*>)));
	if(m_workSpaceTabWgt) m_workSpaceTabWgt->addTab(m_schemeEditor, m_schemeEditor->windowTitle());

	m_scheme = new CScheme(this);
	m_scheme->setObjectName(QStringLiteral("scheme"));
	m_scheme->setSceneRect(0.0, 0.0, 2000.0, 2000.0);
	m_scheme->setNewScheme(true);
	m_scheme->setAlgorithmProtoMng(m_algorithmProtoMng);
	m_schemeEditor->setScheme(m_scheme);

	if(m_engine) m_engine->setScheme(m_scheme);

	if(m_acCursor) m_acCursor->setEnabled(true);
	if(m_acHand) m_acHand->setEnabled(true);
	if(m_acLinking) m_acLinking->setEnabled(true);
	if(m_acCalc) m_acCalc->setEnabled(true);
	if(m_dataWindow)
	{
		connect(m_scheme, SIGNAL(algorithmsSelected(QList<CAlgorithm*>)), m_dataWindow, SLOT(setAlgorithms(QList<CAlgorithm*>)));
		m_dataWindow->setAlgorithms(m_scheme->selectedAlgorithms());
	}
}

void CMainWindow::saveScheme(void)
{
	if(!m_scheme) return;
	if(m_scheme->isNewScheme())
	{
		if(saveSchemeAs()) m_scheme->setNewScheme(false);
	}
	else
	{
		writeScheme(m_scheme, m_scheme->fileName());
	}
}

bool CMainWindow::saveSchemeAs(void)
{
	if(!m_scheme) return false;
	QString fileName = QFileDialog::getSaveFileName(this, "Save as...", m_scheme->fileName());
	if(fileName.isEmpty()) return false;

	m_scheme->setFileName(fileName);
	writeScheme(m_scheme, m_scheme->fileName());
	return true;
}

bool CMainWindow::openScheme(const QString &fileName)
{
	QString schemeName = fileName;
	if(schemeName.isEmpty()) schemeName = QFileDialog::getOpenFileName(this, "Open");
	if(!QFile::exists(schemeName)) return false;

	newScheme();
	if(m_scheme)
	{
		readScheme(m_scheme, schemeName);
		m_scheme->setFileName(schemeName);
	}

	return true;
}

void CMainWindow::closeScheme(void)
{
	if(m_scheme)
	{
		m_scheme->deleteLater();
		m_scheme = 0;
	}
	if(m_acCursor) m_acCursor->setEnabled(false);
	if(m_acHand) m_acHand->setEnabled(false);
	if(m_acLinking) m_acLinking->setEnabled(false);
	if(m_acEditSep) m_acEditSep->setVisible(false);
	if(m_acCopy)
	{
		m_acCopy->setEnabled(false);
		m_acCopy->setVisible(false);
	}
	if(m_acCut)
	{
		m_acCut->setEnabled(false);
		m_acCut->setVisible(false);
	}
	if(m_acDelete)
	{
		m_acDelete->setEnabled(false);
		m_acDelete->setVisible(false);
	}
	if(m_acPaste)
	{
		m_acPaste->setEnabled(false);
		m_acPaste->setVisible(false);
	}
	if(m_acCalc) m_acCalc->setEnabled(false);
	if(m_acElementOptions)
	{
		m_acElementOptions->setEnabled(false);
		m_acElementOptions->setVisible(false);
	}
	if(m_schemeEditor)
	{
		m_schemeEditor->deleteLater();
		m_schemeEditor = 0;
	}
}

void CMainWindow::copy(void)
{
	if(m_schemeEditor) m_schemeEditor->copySelected();
}

void CMainWindow::cut(void)
{
	if(m_schemeEditor) m_schemeEditor->cutSelected();
}

void CMainWindow::del(void)
{
	if(m_schemeEditor) m_schemeEditor->deleteSelected();
}

void CMainWindow::paste(void)
{
	if(m_schemeEditor) m_schemeEditor->pasteSelected();
}

void CMainWindow::elementOptions(void)
{
	if(m_schemeEditor) m_schemeEditor->showElementOptions();
}

void CMainWindow::saveDesktop(const QString &fileName)
{
	if(fileName.isEmpty()) return;

	QSettings desk(fileName, QSettings::IniFormat);
	desk.setValue("MainWindow/geometry", saveGeometry());
	desk.setValue("MainWindow/state", saveState());

	if(m_dataWindow)
	{
		desk.setValue("DataWindow/geometry", m_dataWindow->saveGeometry());
		desk.setValue("DataWindow/state", m_dataWindow->saveState());
		desk.setValue("DataWindow/visible", isDataWindowVisisble());
		if(m_dataWindow->splitter()) desk.setValue("DataWindow/splitter", m_dataWindow->splitter()->saveState());
	}

	QString oldSchemeFileName;
	if(m_scheme)
	{
		if(!m_scheme->isNewScheme() && !m_scheme->fileName().isEmpty())
		{
			oldSchemeFileName = m_scheme->fileName();
		}
	}
	desk.setValue("MainWindow/Old scheme", oldSchemeFileName);
}

void CMainWindow::restoreDesktop(const QString &fileName)
{
	if(fileName.isEmpty()) return;

	QSettings desk(fileName, QSettings::IniFormat);
	bool openResult = false;
	if(isAutoLoadLastScheme())
	{
		QString oldSchemeFileName = desk.value("MainWindow/Old scheme", QString()).toString();
		if(!oldSchemeFileName.isEmpty())
		{
			openResult = openScheme(oldSchemeFileName);
		}
	}
	if(!openResult)
	{
		newScheme();
	}
	if(m_dataWindow)
	{
		m_dataWindow->restoreGeometry(desk.value("DataWindow/geometry").toByteArray());
		m_dataWindow->restoreState(desk.value("DataWindow/state").toByteArray());
		setDataWindowVisible(desk.value("DataWindow/visible").toBool());
		if(m_dataWindow->splitter()) m_dataWindow->splitter()->restoreState(desk.value("DataWindow/splitter").toByteArray());
	}
	restoreGeometry(desk.value("MainWindow/geometry").toByteArray());
	restoreState(desk.value("MainWindow/state").toByteArray());
}

void CMainWindow::saveConfig(const QString &fileName)
{
	if(fileName.isEmpty()) return;

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

	if(m_dataWindow)
	{
		cfg.setValue("DataWindow/autoRefresh", m_dataWindow->isAutoRefresh());
		cfg.setValue("DataWindow/autoRefreshInterval", m_dataWindow->autoRefreshInterval());
	}

	if(m_engine)
	{
		if(m_engine->framer())
		{
			cfg.setValue("Engine/startTime", m_engine->framer()->startTime());
			cfg.setValue("Engine/timeStep", m_engine->framer()->timeStep());
			cfg.setValue("Engine/endTime", m_engine->framer()->endTime());
		}
	}
}

void CMainWindow::restoreConfig(const QString &fileName)
{
	if(fileName.isEmpty()) return;

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

	if(m_dataWindow)
	{
		m_dataWindow->setAutoRefresh(cfg.value("DataWindow/autoRefresh", false).toBool());
		m_dataWindow->setAutoRefreshInterval(cfg.value("DataWindow/autoRefreshInterval", 5).toInt());
	}

	if(m_engine)
	{
		if(m_engine->framer())
		{
			m_engine->framer()->setStartTime(cfg.value("Engine/startTime", 0.0).toDouble());
			m_engine->framer()->setTimeStep(cfg.value("Engine/timeStep", 0.01).toDouble());
			m_engine->framer()->setEndTime(cfg.value("Engine/endTime", 10.0).toDouble());
		}
	}
}