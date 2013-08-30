#include "cmainwindow.h"

#include <QApplication>
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

#include "coptionswindow.h"
#include "datawindow/cdatawindow.h"
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
void CMainWindow::setupToolBars(void)
{
	QToolBar *tbMain = addToolBar("Main");
	tbMain->setObjectName(QStringLiteral("tbMain"));
	tbMain->addAction(tr("New scheme"), this, SLOT(newScheme()));
	tbMain->addAction(tr("Open"), this, SLOT(openScheme()));
	tbMain->addAction(tr("Save schme"), this, SLOT(saveScheme()));
	tbMain->addAction(tr("Save scheme as..."), this, SLOT(saveSchemeAs()));
	tbMain->addAction(tr("Close scheme"), this, SLOT(closeScheme()));
	tbMain->addAction(tr("Options"), this, SLOT(showOptions()));
	tbMain->addAction(tr("Show data"), this, SLOT(showData()));
	if(m_engine)
	{
		m_acCalc = tbMain->addAction("Calc", m_engine, SLOT(calc()));
		m_acCalc->setObjectName(QStringLiteral("acCalc"));
		m_acCalc->setEnabled((m_scheme != 0));
	}

	tbMain->addSeparator();

	m_acGrSchemeMouseMode = new QActionGroup(this);
	m_acGrSchemeMouseMode->setObjectName(QStringLiteral("acGrSchemeMouseMode"));
	m_acGrSchemeMouseMode->setExclusive(true);

	m_acCursor = new QAction(tr("cursor"), this);
	m_acCursor->setObjectName(QStringLiteral("acCursor"));
	m_acCursor->setCheckable(true);
	connect(m_acCursor, SIGNAL(triggered(bool)), this, SLOT(onCursorTriggered(bool)));
	m_acCursor->trigger();
	m_acGrSchemeMouseMode->addAction(m_acCursor);

	m_acHand = new QAction(tr("hand"), this);
	m_acHand->setObjectName(QStringLiteral("acHand"));
	m_acHand->setCheckable(true);
	connect(m_acHand, SIGNAL(triggered(bool)), this, SLOT(onHandTriggered(bool)));
	m_acGrSchemeMouseMode->addAction(m_acHand);

	m_acLinking = new QAction(tr("link"), this);
	m_acLinking->setObjectName(QStringLiteral("acLinking"));
	m_acLinking->setCheckable(true);
	connect(m_acLinking, SIGNAL(triggered(bool)), this, SLOT(onLinkingTriggered(bool)));
	m_acGrSchemeMouseMode->addAction(m_acLinking);

	tbMain->addActions(m_acGrSchemeMouseMode->actions());
	if(m_schemeEditor) m_schemeEditor->addActions(m_acGrSchemeMouseMode->actions());
}

void CMainWindow::setupStatusBar(void)
{
	statusBar()->showMessage(tr("Ready"));
}

void CMainWindow::writeScheme(CScheme *scheme, const QString &fileName)
{
	if(!scheme || fileName.isEmpty()) return;

	QFile fileHandler(fileName);
	if(!fileHandler.open(QIODevice::WriteOnly)) return;
	QTextStream(&fileHandler) << scheme->toXMLDom(scheme->elements(), scheme).toString();
	fileHandler.close();
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

	scheme->addElements(scheme->fromXMLDom(domDoc, scheme));
}

void CMainWindow::closeEvent(QCloseEvent *event)
{
	if(isAutoSaveLastScheme()) saveScheme();
	if(isAutoSaveDesktop()) saveDesktop("desktop.ini");
	if(isAutoSaveConfig()) saveConfig("config.ini");

	QMainWindow::closeEvent(event);
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
	m_gridAlign = true;

	m_acCalc = 0;
	m_acGrSchemeMouseMode = 0;
	m_acCursor = 0;
	m_acHand = 0;
	m_acLinking = 0;
	m_workSpaceTabWgt = 0;
	m_dataWindow = 0;
	m_algorithmProtoMng = 0;
	m_algProtoView = 0;
	m_algProtoViewDock = 0;
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

	m_algProtoView = new CAlgProtoView(m_algorithmProtoMng, Qt::Vertical);
	m_algProtoView->setObjectName(QStringLiteral("algProtoView"));
	m_algProtoView->setWindowTitle(tr("Algorithm prototypes"));
	m_algProtoViewDock = new QDockWidget(m_algProtoView->windowTitle());
	m_algProtoViewDock->setObjectName(QStringLiteral("algProtoViewDock"));
	m_algProtoViewDock->setAllowedAreas(Qt::AllDockWidgetAreas);
	m_algProtoViewDock->setWidget(m_algProtoView);
	m_algProtoViewDock->setTitleBarWidget(new QLabel(""));
	connect(m_algProtoViewDock, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT(onAlgProtosViewDockLocationChanged(Qt::DockWidgetArea)));
	connect(m_algProtoViewDock, SIGNAL(topLevelChanged(bool)), this, SLOT(onAlgProtosViewDockTopLevelChanged(bool)));
	addDockWidget(Qt::LeftDockWidgetArea, m_algProtoViewDock);

	m_engine = new CEngine(this);
	m_engine->setObjectName(QStringLiteral("engine"));
	connect(m_engine, SIGNAL(calcStopped()), m_dataWindow, SLOT(flushBuffers()));

	setupToolBars();
	setupStatusBar();

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

void CMainWindow::setGridAlign(const bool &gridAlign)
{
	if(m_gridAlign == gridAlign) return;

	m_gridAlign = gridAlign;
	if(m_schemeEditor) m_schemeEditor->setGridAlign(m_gridAlign);
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

void CMainWindow::onSchemeEditorMouseReleased(const QPointF &pos)
{
	Q_UNUSED(pos)
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

void CMainWindow::showData(void)
{
	if(m_dataWindow)
	{
		m_dataWindow->show();
	}
}

void CMainWindow::newScheme(void)
{
	if(m_scheme) closeScheme();

	m_scheme = new CScheme(this);
	m_scheme->setObjectName(QStringLiteral("scheme"));
	m_scheme->setSceneRect(0.0, 0.0, 1000.0, 1000.0);
	m_scheme->setNewScheme(true);
	m_scheme->setAlgorithmProtoMng(m_algorithmProtoMng);

	m_schemeEditor = new CSchemeEditor(this);
	m_schemeEditor->setObjectName(QStringLiteral("schemeEditor"));
	m_schemeEditor->setupGrid(gridColor(), gridBkGndColor(), gridStep(), isGridAlign());
	m_schemeEditor->setScheme(m_scheme);
	connect(m_schemeEditor, SIGNAL(mouseReleased(QPointF)), this, SLOT(onSchemeEditorMouseReleased(QPointF)));
	connect(m_schemeEditor, SIGNAL(windowTitleChanged()), this, SLOT(onSchemeEditorWindowTitleChanged()));
	if(m_workSpaceTabWgt) m_workSpaceTabWgt->addTab(m_schemeEditor, m_schemeEditor->windowTitle());

	if(m_engine)
	{
		m_engine->setScheme(m_scheme);
		if(m_acCalc) m_acCalc->setEnabled(true);
	}
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
		m_scheme->setNewScheme(false);
	}

	return true;
}

void CMainWindow::closeScheme(void)
{
	if(m_scheme)
	{
		m_scheme->deleteLater();
		m_scheme = 0;
		if(m_acCalc) m_acCalc->setEnabled(false);
	}
	if(m_schemeEditor)
	{
		m_schemeEditor->deleteLater();
		m_schemeEditor = 0;
	}
}

void CMainWindow::saveDesktop(const QString &fileName)
{
	if(fileName.isEmpty()) return;

	QSettings desk(fileName, QSettings::IniFormat);
	desk.setValue("MainWindow/geometry", saveGeometry());
	desk.setValue("MainWindow/state", saveState());

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
	cfg.setValue("SchemeEditor/gridAlign", isGridAlign());

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
	setGridAlign(cfg.value("SchemeEditor/gridAlign", m_gridAlign).toBool());

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