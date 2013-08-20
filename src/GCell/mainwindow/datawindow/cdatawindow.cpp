#include "cdatawindow.h"

#include <QTabWidget>
#include <QTableView>

#include "../../scheme/algorithm/calgorithm.h"
#include "../../scheme/portal/cargument.h"
#include "../../scheme/portal/cresult.h"
#include "cdataplot.h"
#include "cdatatable.h"

/*!
 * \class CDataWindow
 */
void CDataWindow::addPortal(CPortal *portal)
{
	if(!portal) return;
	if(!portal->buffer()) return;

	if(qobject_cast<CArgument*>(portal))
	{
		if(m_argDataPlot) m_argDataPlot->addPortal(portal);
		if(m_argDataTable) m_argDataTable->addPortal(portal);
	}
	else if(qobject_cast<CResult*>(portal))
	{
		if(m_resDataPlot) m_resDataPlot->addPortal(portal);
		if(m_resDataTable) m_resDataTable->addPortal(portal);
	}
}

void CDataWindow::addArguments(const QList<CPortal*> &arguments)
{
	if(m_argDataPlot) m_argDataPlot->addPortals(arguments);
	if(m_argDataTable) m_argDataTable->addPortals(arguments);
}

void CDataWindow::addResults(const QList<CPortal*> &results)
{
	if(m_resDataPlot) m_resDataPlot->addPortals(results);
	if(m_resDataTable) m_resDataTable->addPortals(results);
}

void CDataWindow::addPortals(const QList<CAlgorithm*> &algorithms)
{
	foreach(CAlgorithm *alg, algorithms)
	{
		if(!alg) continue;
		addArguments(alg->argPortals());
		addResults(alg->resPortals());
	}
}

CDataWindow::CDataWindow(QWidget *parent) : QMainWindow(parent)
{
	setObjectName(QStringLiteral("CDataWindow"));
	setWindowFlags(Qt::Dialog);

	m_argResTabs = 0;
	m_argTabs = 0;
	m_resTabs = 0;
    m_argDataPlot = 0;
    m_argDataTable = 0;
    m_resDataPlot = 0;
    m_resDataTable = 0;

	m_argResTabs = new QTabWidget();
	m_argResTabs->setObjectName(QStringLiteral("argResTabs"));
	setCentralWidget(m_argResTabs);

	m_argTabs = new QTabWidget();
	m_argTabs->setObjectName(QStringLiteral("argTabs"));
	m_argTabs->setWindowTitle(tr("Arguments"));
	m_argResTabs->addTab(m_argTabs, m_argTabs->windowTitle());

	m_resTabs = new QTabWidget();
	m_resTabs->setObjectName("resTabs");
	m_resTabs->setWindowTitle(tr("Results"));
	m_argResTabs->addTab(m_resTabs, m_resTabs->windowTitle());

    m_argDataPlot = new CDataPlot();
    m_argDataPlot->setObjectName(QStringLiteral("argDataPlot"));
    m_argDataPlot->setWindowTitle(tr("Arguments plot"));
    m_argTabs->addTab(m_argDataPlot, m_argDataPlot->windowTitle());

    m_resDataPlot = new CDataPlot();
    m_resDataPlot->setObjectName(QStringLiteral("resDataPlot"));
    m_resDataPlot->setWindowTitle(tr("Results plot"));
    m_resTabs->addTab(m_resDataPlot, m_resDataPlot->windowTitle());

    QTableView *argTabView = new QTableView();
    argTabView->setObjectName(QStringLiteral("argTabView"));
    argTabView->setWindowTitle("Arguments table");
    m_argTabs->addTab(argTabView, argTabView->windowTitle());

    QTableView *resTabView = new QTableView();
    resTabView->setObjectName(QStringLiteral("resTabView"));
    resTabView->setWindowTitle("Results table");
    m_resTabs->addTab(resTabView, resTabView->windowTitle());

    m_argDataTable = new CDataTable(this);
    m_argDataTable->setObjectName(QLatin1String("argDataTable"));
    argTabView->setModel(m_argDataTable);

    m_resDataTable = new CDataTable(this);
    m_resDataTable->setObjectName(QLatin1String("resDataTable"));
    resTabView->setModel(m_resDataTable);
}

void CDataWindow::onAlgorithmDestroyed(QObject *objAlgorithm)
{
	if(!objAlgorithm) return;
	if(m_algorithms.contains((CAlgorithm*)objAlgorithm)) m_algorithms.removeOne((CAlgorithm*)objAlgorithm);
}

void CDataWindow::onPortalAdded(CPortal *portal)
{
	if(!portal) return;
	addPortal(portal);
	updateViews();
}

void CDataWindow::updateViews(void)
{
	if(m_argDataPlot) m_argDataPlot->refresh();
	if(m_argDataTable) m_argDataTable->refresh();
	if(m_resDataPlot) m_resDataPlot->refresh();
	if(m_resDataTable) m_resDataTable->refresh();
}

void CDataWindow::setAlgorithms(const QList<CAlgorithm*> &algorithms)
{
	foreach(CAlgorithm *alg, m_algorithms)
	{
		if(!qobject_cast<CAlgorithm*>(alg)) continue;

		disconnect(alg, SIGNAL(portalAdded(CPortal*)), this, SLOT(onPortalAdded(CPortal*)));
		disconnect(alg, SIGNAL(destroyed(QObject*)), this, SLOT(onAlgorithmDestroyed(QObject*)));
	}
	if(m_argDataPlot) m_argDataPlot->clearPortals();
	if(m_argDataTable) m_argDataTable->clearPortals();
	if(m_resDataPlot) m_resDataPlot->clearPortals();
	if(m_resDataTable) m_resDataTable->clearPortals();

	m_algorithms = algorithms;
	foreach(CAlgorithm *alg, m_algorithms)
	{
		if(!qobject_cast<CAlgorithm*>(alg)) continue;

		connect(alg, SIGNAL(portalAdded(CPortal*)), this, SLOT(onPortalAdded(CPortal*)));
		connect(alg, SIGNAL(destroyed(QObject*)), this, SLOT(onAlgorithmDestroyed(QObject*)));
	}
	if(isVisible())
	{
		addPortals(m_algorithms);
		updateViews();
	}
}

void CDataWindow::flushBuffers(void)
{
	if(m_argDataPlot) m_argDataPlot->flush();
	if(m_argDataTable) m_argDataTable->flush();
	if(m_resDataPlot) m_resDataPlot->flush();
	if(m_resDataTable) m_resDataTable->flush();
}

void CDataWindow::setVisible(bool visible)
{
	QMainWindow::setVisible(visible);
	if(isVisible())
	{
		addPortals(m_algorithms);
		updateViews();
	}
	else
	{
		addPortals(QList<CAlgorithm*>());
	}
}