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

#include "calgorithmprotostoolbar.h"
#include "coptionswindow.h"
#include "datawindow/cdatawindow.h"
#include "../scheme/algorithmproto/calgorithmproto.h"
#include "../scheme/algorithmproto/calgorithmprotomng.h"
#include "../scheme/cscheme.h"
#include "../scheme/cschemeeditor.h"
#include "../engine/cengine.h"
#include "../engine/ctimeframegenerator.h"
#include "../algorithms/CSV/CSVIn/ccsvin.h"
#include "../algorithms/TAC/StepExaction/cstepexcitation.h"
#include "../algorithms/General/Amp/camp.h"
#include "../algorithms/General/Sum/csum.h"
#include "../algorithms/TAC/TransLink/ctranslink.h"
#include "../algorithms/CSV/CSVOut/ccsvout.h"

/*!
 * \class CMainWindow
 */
void CMainWindow::setupToolBars(void)
{
    QToolBar *tbMain = addToolBar("Main");
    tbMain->setObjectName(QStringLiteral("tbMain"));
    tbMain->addAction("New scheme", this, SLOT(newScheme()));
	tbMain->addAction("Open", this, SLOT(openScheme()));
	tbMain->addAction("Save schme", this, SLOT(saveScheme()));
	tbMain->addAction("Save scheme as...", this, SLOT(saveSchemeAs()));
    tbMain->addAction("Close scheme", this, SLOT(closeScheme()));
	tbMain->addAction(tr("Options"), this, SLOT(showOptions()));
	tbMain->addAction("Show data", this, SLOT(showData()));
    if(m_engine)
    {
        if(m_acCalc) m_acCalc->deleteLater();

        m_acCalc = tbMain->addAction("Calc", m_engine, SLOT(calc()));
        m_acCalc->setObjectName(QStringLiteral("acCalc"));
        m_acCalc->setEnabled((m_scheme != 0));
    }

	if(m_tbAlgorithmProtos) m_tbAlgorithmProtos->deleteLater();
	m_tbAlgorithmProtos = new CAlgorithmProtosToolBar(m_algorithmProtoMng, this);
	m_tbAlgorithmProtos->setObjectName(QStringLiteral("tbAlgorithmProtos"));
    if(m_acCursor)
    {
		m_tbAlgorithmProtos->addManaginAction(m_acCursor);
		m_tbAlgorithmProtos->setDefaultAction(m_acCursor);
        m_acCursor->trigger();
    }
	if(m_acHand) m_tbAlgorithmProtos->addManaginAction(m_acHand);
	if(m_acLinking) m_tbAlgorithmProtos->addManaginAction(m_acLinking);
	addToolBar(m_tbAlgorithmProtos);
}

void CMainWindow::writeScheme(CScheme *scheme, const QString &fileName)
{
	if(!scheme || fileName.isEmpty()) return;

	QFile fileHandler(fileName);
	if(!fileHandler.open(QIODevice::WriteOnly)) return;
	QTextStream(&fileHandler) << scheme->toXMLDom(scheme->elements()).toString(); /*xmlScheme.schemeToDom(scheme).toString();*/
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

	scheme->addElements(scheme->fromXMLDom(domDoc));
}

void CMainWindow::closeEvent(QCloseEvent *event)
{
    saveConfig("config.ini");
    QMainWindow::closeEvent(event);
}

CMainWindow::CMainWindow(QWidget *parent) : QMainWindow(parent)
{
    setObjectName("CMainWindow");

    m_acCalc = 0;
    m_acCursor = 0;
    m_acHand = 0;
	m_acLinking = 0;
	m_tbAlgorithmProtos = 0;
	m_dataWindow = 0;
	m_algorithmProtoMng = 0;
	m_schemeEditor = 0;
    m_scheme = 0;
    m_engine = 0;

    m_acCursor = new QAction(tr("cursor"), this);
    m_acCursor->setObjectName(QStringLiteral("acCursor"));
    connect(m_acCursor, SIGNAL(triggered(bool)), this, SLOT(onCursorTriggered(bool)));

    m_acHand = new QAction(tr("hand"), this);
    m_acHand->setObjectName(QStringLiteral("acHand"));
    connect(m_acHand, SIGNAL(triggered(bool)), this, SLOT(onHandTriggered(bool)));

	m_acLinking = new QAction(tr("link"), this);
	m_acLinking->setObjectName(QStringLiteral("acLinking"));
	connect(m_acLinking, SIGNAL(triggered(bool)), this, SLOT(onLinkingTriggered(bool)));

	m_dataWindow = new CDataWindow(this);
	m_dataWindow->setObjectName(QStringLiteral("dataWindow"));

	m_algorithmProtoMng = new CAlgorithmProtoMng(this);
	m_algorithmProtoMng->setObjectName(QStringLiteral("algorithmProtoMng"));
	m_algorithmProtoMng->addProto(tr("CSV In"), &CCSVIn::staticMetaObject);
	m_algorithmProtoMng->addProto(tr("A(t)"), &CStepExcitation::staticMetaObject);
	m_algorithmProtoMng->addProto(tr("Amp"), &CAmp::staticMetaObject);
    m_algorithmProtoMng->addProto(tr("Sum"), &CSum::staticMetaObject);
	m_algorithmProtoMng->addProto(tr("TL"), &CTransLink::staticMetaObject);
	m_algorithmProtoMng->addProto(tr("CSV Out"), &CCSVOut::staticMetaObject);
    connect(m_algorithmProtoMng, SIGNAL(algorithmProtoSelected(CAlgorithmProto*)), this, SLOT(onAlgorithmProtoSelected(CAlgorithmProto*)));

	m_schemeEditor = new CSchemeEditor(this);
	m_schemeEditor->setObjectName(QStringLiteral("schemeEditor"));
	m_schemeEditor->addAction(m_acCursor);
	m_schemeEditor->addAction(m_acHand);
	m_schemeEditor->addAction(m_acLinking);
	m_schemeEditor->setSceneRect(0.0, 0.0, 1000.0, 1000.0);
	connect(m_schemeEditor, SIGNAL(mouseReleased(QPointF)), this, SLOT(onSchemeEditorMouseReleased(QPointF)));
	setCentralWidget(m_schemeEditor);

    m_engine = new CEngine(this);
    m_engine->setObjectName(QStringLiteral("engine"));
	connect(m_engine, SIGNAL(calcStopped()), m_dataWindow, SLOT(flushBuffers()));

    setupToolBars();
    newScheme();
    restoreConfig("config.ini");
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
}

void CMainWindow::showOptions(void)
{
	COptionsWindow optWnd(this);
	if(m_engine && m_engine->framer())
	{
		optWnd.setStartTime(m_engine->framer()->startTime());
		optWnd.setTimeStep(m_engine->framer()->timeStep());
		optWnd.setEndTime(m_engine->framer()->endTime());
	}
	int dlgResult = optWnd.exec();
	switch(dlgResult)
	{
		case QDialog::Accepted:
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

void CMainWindow::updateTitle(void)
{
	QString newTitle = QApplication::applicationName();
	if(m_scheme) newTitle += " [" + m_scheme->fileName() + "]";
	setWindowTitle(newTitle);
}

void CMainWindow::newScheme(void)
{
    if(m_scheme) closeScheme();

    m_scheme = new CScheme(this);
    m_scheme->setObjectName(QStringLiteral("scheme"));
	m_scheme->setNewScheme(true);
    m_scheme->setAlgorithmProtoMng(m_algorithmProtoMng);
	connect(m_scheme, SIGNAL(fileNameChanged(QString)), this, SLOT(updateTitle()));
	if(m_schemeEditor) m_schemeEditor->setScheme(m_scheme);
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
	updateTitle();
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

void CMainWindow::openScheme(void)
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open");
	if(!QFile::exists(fileName)) return;

	newScheme();
	if(m_scheme)
	{
		readScheme(m_scheme, fileName);
		m_scheme->setFileName(fileName);
		m_scheme->setNewScheme(false);
	}
}

void CMainWindow::closeScheme(void)
{
    if(m_scheme)
    {
        m_scheme->deleteLater();
        m_scheme = 0;
        if(m_acCalc) m_acCalc->setEnabled(false);
    }
	updateTitle();
}

void CMainWindow::saveConfig(const QString &fileName)
{
    if(fileName.isEmpty()) return;

    QSettings cfg(fileName, QSettings::IniFormat);
    cfg.setValue("MainWindow/geometry", saveGeometry());
    cfg.setValue("MainWindow/state", saveState());
}

void CMainWindow::restoreConfig(const QString &fileName)
{
    if(fileName.isEmpty()) return;

    QSettings cfg(fileName, QSettings::IniFormat);
    restoreGeometry(cfg.value("MainWindow/geometry").toByteArray());
	restoreState(cfg.value("MainWindow/state").toByteArray());
}