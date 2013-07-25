#include "cmainwindow.h"

#include <QToolBar>
#include <QAction>
#include <QEvent>
#include <QMouseEvent>
#include <QSettings>

#include "calgorithmprotostoolbar.h"
#include "../scheme/calgorithmproto.h"
#include "../scheme/calgorithmprotomng.h"
#include "../scheme/cscheme.h"
#include "../scheme/cschemeeditor.h"
#include "../engine/cengine.h"
#include "../algorithms/CSV/CSVIn/ccsvin.h"
#include "../algorithms/General/Amp/camp.h"
#include "../algorithms/CSV/CSVOut/ccsvout.h"

/*!
 * \class CMainWindow
 */
void CMainWindow::setupToolBars(void)
{
    QToolBar *tbMain = addToolBar("Main");
    tbMain->setObjectName(QStringLiteral("tbMain"));
    tbMain->addAction("New scheme", this, SLOT(newScheme()));
    tbMain->addAction("Close scheme", this, SLOT(closeScheme()));
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

	m_algorithmProtoMng = new CAlgorithmProtoMng(this);
	m_algorithmProtoMng->setObjectName(QStringLiteral("algorithmProtoMng"));
	m_algorithmProtoMng->addProto(tr("CSV In"), &CCSVIn::staticMetaObject);
    m_algorithmProtoMng->addProto(tr("Amp"), &CAmp::staticMetaObject);
    m_algorithmProtoMng->addProto(tr("CSV Out"), &CCSVOut::staticMetaObject);
    connect(m_algorithmProtoMng, SIGNAL(algorithmProtoSelected(CAlgorithmProto*)), this, SLOT(onAlgorithmProtoSelected(CAlgorithmProto*)));

	m_schemeEditor = new CSchemeEditor(this);
	m_schemeEditor->setObjectName(QStringLiteral("schemeEditor"));
	m_schemeEditor->addAction(m_acCursor);
	m_schemeEditor->addAction(m_acHand);
	m_schemeEditor->setSceneRect(0.0, 0.0, 1000.0, 1000.0);
	connect(m_schemeEditor, SIGNAL(mouseReleased(QPointF)), this, SLOT(onSchemeEditorMouseReleased(QPointF)));
	setCentralWidget(m_schemeEditor);

    m_engine = new CEngine(this);
    m_engine->setObjectName(QStringLiteral("engine"));

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

void CMainWindow::newScheme(void)
{
    if(m_scheme) closeScheme();

    m_scheme = new CScheme(this);
    m_scheme->setObjectName(QStringLiteral("scheme"));
    m_scheme->setAlgorithmProtoMng(m_algorithmProtoMng);
	if(m_schemeEditor) m_schemeEditor->setScene(m_scheme);
	if(m_engine)
	{
		m_engine->setScheme(m_scheme);
		if(m_acCalc) m_acCalc->setEnabled(true);
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