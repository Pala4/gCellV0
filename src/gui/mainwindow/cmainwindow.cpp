#include "cmainwindow.h"

#include <QToolBar>
#include <QAction>
#include <QEvent>
#include <QMouseEvent>

#include "celementprotostoolbar.h"
#include "../schemeview/celementproto.h"
#include "../schemeview/celementprotomng.h"
#include "../../core/scheme/cscheme.h"
#include "../schemeview/cschemeview.h"
#include "../schemeview/cschememodel.h"
#include "../schemeview/cgrelement.h"
#include "../../core/engine/cengine.h"
#include "../../elements/CSV/CSVIn/ccsvin.h"

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

	if(m_tbElementProtos) m_tbElementProtos->deleteLater();
	m_tbElementProtos = new CElementProtosToolBar(m_elementProtoMng, this);
	m_tbElementProtos->setObjectName(QStringLiteral("tbElementProtos"));
    if(m_acCursor)
    {
        m_tbElementProtos->addManaginAction(m_acCursor);
        m_tbElementProtos->setDefaultAction(m_acCursor);
        m_acCursor->setChecked(true);
    }
    if(m_acHand) m_tbElementProtos->addManaginAction(m_acHand);
    addToolBar(m_tbElementProtos);
}

CMainWindow::CMainWindow(QWidget *parent) : QMainWindow(parent)
{
    setObjectName("CMainWindow");

    m_acCalc = 0;
    m_acCursor = 0;
    m_acHand = 0;
    m_tbElementProtos = 0;
    m_elementProtoMng = 0;
    m_schemeView = 0;
    m_scheme = 0;
    m_engine = 0;

    m_acCursor = new QAction(tr("cursor"), this);
    m_acCursor->setObjectName(QStringLiteral("acCursor"));
    connect(m_acCursor, SIGNAL(triggered(bool)), this, SLOT(onCursorTriggered(bool)));
    connect(m_acCursor, SIGNAL(toggled(bool)), this, SLOT(onCursorTriggered(bool)));

    m_acHand = new QAction(tr("hand"), this);
    m_acHand->setObjectName(QStringLiteral("acHand"));

    m_elementProtoMng = new CElementProtoMng(this);
    m_elementProtoMng->setObjectName(QStringLiteral("elementProtoMng"));
    m_elementProtoMng->addProto("csvin", tr("CSV In"), &CCSVIn::staticMetaObject);
    connect(m_elementProtoMng, SIGNAL(elementProtoSelected(CElementProto*)), this, SLOT(onElementProtoSelected(CElementProto*)));

    m_schemeModel = new CSchemeModel(this);
    m_schemeModel->setObjectName(QStringLiteral("schemeModel"));

    m_schemeView = new CSchemeView(this);
    m_schemeView->setObjectName(QStringLiteral("schemeView"));
    m_schemeView->setScene(m_schemeModel);
    m_schemeView->addAction(m_acCursor);
    m_schemeView->addAction(m_acHand);
    m_schemeView->setSceneRect(0.0, 0.0, 1000.0, 1000.0);
    connect(m_schemeView, SIGNAL(queryAddElement(QPointF)), this, SLOT(addElement(QPointF)));
    connect(m_schemeView, SIGNAL(mouseReleased(QPointF)), this, SLOT(onSchemeEditorMouseReleased(QPointF)));
    setCentralWidget(m_schemeView);

    m_engine = new CEngine(this);
    m_engine->setObjectName(QStringLiteral("engine"));

    setupToolBars();
    newScheme();
}

void CMainWindow::onCursorTriggered(const bool &checked)
{
    if(checked && m_schemeView) m_schemeView->setMouseMode(CSchemeView::MoveSelectMode);
}

void CMainWindow::onElementProtoSelected(CElementProto *elementProto)
{
    Q_UNUSED(elementProto)
    if(m_schemeView) m_schemeView->setMouseMode(CSchemeView::AddElementMode);
}

void CMainWindow::onSchemeEditorMouseReleased(const QPointF &pos)
{
    Q_UNUSED(pos)
    if(m_elementProtoMng && m_elementProtoMng->selectedElementProto()) m_elementProtoMng->setSelectedElementProto(0);
}

void CMainWindow::newScheme(void)
{
    if(!m_schemeModel || !m_engine) return;
    if(m_scheme) closeScheme();

    m_scheme = new CScheme(this);
    m_scheme->setObjectName(QStringLiteral("scheme"));
    m_schemeModel->setScheme(m_scheme);
    m_engine->setScheme(m_scheme);
    if(m_acCalc) m_acCalc->setEnabled(true);
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

void CMainWindow::addElement(const QPointF &pos)
{
    if(!m_elementProtoMng || !m_scheme) return;
    if(!m_elementProtoMng->selectedElementProto()) return;

    CElement *element = m_elementProtoMng->selectedElementProto()->createElement();
    if(!element) return;
    element->setPos(pos);

    m_scheme->addElement(element);
}