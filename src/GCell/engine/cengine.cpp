#include "cengine.h"

#include "../scheme/cscheme.h"
#include "../scheme/algorithm/calgorithm.h"
#include "../scheme/algorithm/cdatasource.h"
#include "../scheme/portal/cresult.h"
#include "../scheme/portal/cargument.h"
#include "../scheme/link/clink.h"
#include "../scheme/elementlistutil.h"

/*!
 * \class CTraceData
 */
void CTraceData::addInspectedAlgorithm(CAlgorithm *algorithm)
{
	if(!algorithm) return;
	if(m_inspectedAlgorithms.contains(algorithm)) return;

	m_inspectedAlgorithms << algorithm;
}

void CTraceData::removeInspectedAlgorithm(CAlgorithm *algorithm)
{
	if(!algorithm) return;
	if(!m_inspectedAlgorithms.contains(algorithm)) return;

	m_inspectedAlgorithms.removeOne(algorithm);
}

void CTraceData::addLoopBackPortal(CPortal *portal)
{
	if(!portal) return;
	if(m_loopBackPortals.contains(portal)) return;

	m_loopBackPortals << portal;
}

void CTraceData::release(void)
{
	clearDataSources();
	clearInspectedAlgorithm();
	clearLoopBackPortals();
}

/*!
 * \class CEngine
 */
void CEngine::traceArgument(CArgument *argument)
{
	if(!argument) return;
	if(m_traceData.isLoopBackPortal(argument)) return;

	CAlgorithm *alg = dynamic_cast<CAlgorithm*>(argument->parentItem());
	if(m_traceData.isInspected(alg))
	{
		m_traceData.addLoopBackPortal(argument);
		argument->setLoopBackPortal(true);
		return;
	}
	if(alg) traceAlgorithm(alg);
}

void CEngine::traceLink(CLink *link)
{
	if(!link) return;
	traceArgument(dynamic_cast<CArgument*>(link->argument()));
}

void CEngine::traceResult(CResult *result)
{
	if(!result) return;
	foreach(CLink* link, result->links())
	{
		if(!link) continue;
		traceLink(link);
	}
}

void CEngine::traceAlgorithm(CAlgorithm *algorithm)
{
	if(!algorithm) return;
	m_traceData.addInspectedAlgorithm(algorithm);
	QList<CResult*> ress = getElements<CResult*, CPortal*>(algorithm->portals());
	foreach(CResult *res, ress)
	{
		if(!res) continue;
		traceResult(res);
	}
	m_traceData.removeInspectedAlgorithm(algorithm);
}

void CEngine::traceScheme(void)
{
	if(!m_scheme) return;

	m_traceData.setDataSources(getElements<CDataSource*, QGraphicsItem*>(m_scheme->items()));
	foreach(CDataSource *ds, m_traceData.dataSources())
	{
		if(!ds) continue;
		QList<CArgument*> args = getElements<CArgument*, CPortal*>(ds->portals());
		foreach(CArgument *arg, args)
		{
			if(!arg) continue;
			m_traceData.addLoopBackPortal(arg);
			arg->setLoopBackPortal(true);
		}
	}
	foreach(CDataSource *ds, m_traceData.dataSources())
	{
		if(ds) traceAlgorithm(ds);
	}
}

CEngine::CEngine(QObject *parent) : QObject(parent)
{
    setObjectName(QStringLiteral("CEngine"));

    m_scheme = 0;
}

void CEngine::setScheme(CScheme *scheme)
{
    if(m_scheme && (m_scheme == scheme)) return;

    if(m_scheme)
    {
        disconnect(m_scheme, SIGNAL(destroyed()), this, SLOT(onSchemeDestroyed()));
    }

    m_scheme = scheme;

    if(m_scheme)
    {
        connect(m_scheme, SIGNAL(destroyed()), this, SLOT(onSchemeDestroyed()));
    }
}

void CEngine::onSchemeDestroyed(void)
{
    m_scheme = 0;
}

void CEngine::calc(void)
{
	if(!m_scheme) return;

	traceScheme();
	for(int timeFrame = 0; timeFrame < 100; ++timeFrame)
	{
		foreach(CDataSource *ds, m_traceData.dataSources())
		{
			if(!ds) continue;
			ds->calc(timeFrame);
		}
		for(int ci = 0; ci < 1000000; ++ci);
	}
	m_traceData.release();
}