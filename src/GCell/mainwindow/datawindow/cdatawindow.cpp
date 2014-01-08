#include "cdatawindow.h"

#include <QTabWidget>
#include <QTableView>
#include <QTreeView>
#include <QHBoxLayout>

#include "../../scheme/algorithm/calgorithm.h"
#include "../../scheme/portal/cargument.h"
#include "../../scheme/portal/cresult.h"
#include "cdataplot.h"
#include "cdatatable.h"
#include "algbuffmodel/calgbuffmodel.h"

/*!
 * \class CDataWindow
 */
void CDataWindow::addPortal(CPortal *portal)
{
//	if(!portal) return;
//	if(!portal->buffer()) return;

//	if(qobject_cast<CArgument*>(portal))
//	{
//		if(m_argDataPlot) m_argDataPlot->addPortal(portal);
//		if(m_argDataTable) m_argDataTable->addPortal(portal);
//	}
//	else if(qobject_cast<CResult*>(portal))
//	{
//		if(m_dataPlot) m_dataPlot->addPortal(portal);
//		if(m_dataTable) m_dataTable->addPortal(portal);
//	}
}

void CDataWindow::addArguments(const QList<CPortal*> &arguments)
{
//	if(m_argDataPlot) m_argDataPlot->addPortals(arguments);
//	if(m_argDataTable) m_argDataTable->addPortals(arguments);
}

void CDataWindow::addResults(const QList<CPortal*> &results)
{
	if(m_dataPlot) m_dataPlot->addPortals(results);
	if(m_dataTable) m_dataTable->addPortals(results);
}

void CDataWindow::addPortals(const QList<CAlgorithm*> &algorithms)
{
//	if(algorithms.isEmpty())
//	{
//		if(m_argDataPlot) m_argDataPlot->clearPortals();
//		if(m_argDataTable) m_argDataTable->clearPortals();
//		if(m_dataPlot) m_dataPlot->clearPortals();
//		if(m_dataTable) m_dataTable->clearPortals();
//	}
//	else
//	{
//		foreach(CAlgorithm *alg, algorithms)
//		{
//			if(!alg) continue;
//			addArguments(alg->argPortals());
//			addResults(alg->resPortals());
//		}
//	}
}

CDataWindow::CDataWindow(QWidget *parent) : QMainWindow(parent)
{
	setObjectName(QStringLiteral("CDataWindow"));
	setWindowFlags(Qt::Dialog);

	m_tabWgt = 0;
	m_dataPlot = 0;
	m_dataTable = 0;
	m_algBuffModel = 0;

	QWidget *wgtMain = new QWidget();
	setCentralWidget(wgtMain);

	QHBoxLayout *hblMain = new QHBoxLayout();
	wgtMain->setLayout(hblMain);

	QTreeView *algBuffView = new QTreeView();
	hblMain->addWidget(algBuffView);

	m_tabWgt = new QTabWidget();
	m_tabWgt->setObjectName(QStringLiteral("tabWgt"));
	hblMain->addWidget(m_tabWgt, 1);

	m_dataPlot = new CDataPlot();
	m_dataPlot->setObjectName(QStringLiteral("dataPlot"));
	m_dataPlot->setWindowTitle(tr("Plot"));
	m_tabWgt->addTab(m_dataPlot, m_dataPlot->windowTitle());

	QTableView *tabView = new QTableView();
	tabView->setObjectName(QStringLiteral("tabView"));
	tabView->setWindowTitle("Table");
	m_tabWgt->addTab(tabView, tabView->windowTitle());

	m_dataTable = new CDataTable(this);
	m_dataTable->setObjectName(QLatin1String("dataTable"));
	tabView->setModel(m_dataTable);

	m_algBuffModel = new CAlgBuffModel(this);
	m_algBuffModel->setObjectName(QStringLiteral("algBuffModel"));
	algBuffView->setModel(m_algBuffModel);
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
	if(m_dataPlot) m_dataPlot->refresh();
	if(m_dataTable) m_dataTable->refresh();
}

void CDataWindow::setAlgorithms(const QList<CAlgorithm*> &algorithms)
{
	if(m_algBuffModel) m_algBuffModel->setAlgorithms(algorithms);
//	foreach(CAlgorithm *alg, m_algorithms)
//	{
//		if(!qobject_cast<CAlgorithm*>(alg)) continue;

//		disconnect(alg, SIGNAL(portalAdded(CPortal*)), this, SLOT(onPortalAdded(CPortal*)));
//		disconnect(alg, SIGNAL(destroyed(QObject*)), this, SLOT(onAlgorithmDestroyed(QObject*)));
//	}
//	if(m_argDataPlot) m_argDataPlot->clearPortals();
//	if(m_argDataTable) m_argDataTable->clearPortals();
//	if(m_dataPlot) m_dataPlot->clearPortals();
//	if(m_dataTable) m_dataTable->clearPortals();

//	m_algorithms = algorithms;
//	foreach(CAlgorithm *alg, m_algorithms)
//	{
//		if(!qobject_cast<CAlgorithm*>(alg)) continue;

//		connect(alg, SIGNAL(portalAdded(CPortal*)), this, SLOT(onPortalAdded(CPortal*)));
//		connect(alg, SIGNAL(destroyed(QObject*)), this, SLOT(onAlgorithmDestroyed(QObject*)));
//	}
//	if(isVisible())
//	{
//		addPortals(m_algorithms);
//		updateViews();
//	}
}

void CDataWindow::flushBuffers(void)
{
	if(m_dataPlot) m_dataPlot->flush();
	if(m_dataTable) m_dataTable->flush();
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