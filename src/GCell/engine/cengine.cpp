#include "cengine.h"

#include <qmath.h>

#include "ctimeframegenerator.h"
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

	m_framer = 0;
    m_scheme = 0;

	m_framer = new CTimeFrameGenerator(0.0, 0.01, 10.0, this);
	m_framer->setObjectName(QStringLiteral("framer"));
	connect(m_framer, SIGNAL(newTimeFrame(stTimeLine)), this, SLOT(onNewTimeFrame(stTimeLine)));
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

void CEngine::onNewTimeFrame(const stTimeLine &timeLine)
{
	foreach(CDataSource *ds, m_traceData.dataSources())
	{
		if(ds) ds->calc(timeLine);
	}
}

void CEngine::onSchemeDestroyed(void)
{
    m_scheme = 0;
}

void CEngine::calc(void)
{
	if(!m_scheme) return;
	if(!m_framer) return;

	emit calcStarted();

    foreach(CElement *element, m_scheme->elements())
    {
		element->beforeCalc(m_framer->startTime(), m_framer->timeStep(), m_framer->endTime());
    }
	traceScheme();

	m_framer->start();

	m_traceData.release();
    foreach(CElement *element, m_scheme->elements())
    {
        element->afterCalc();
    }

	emit calcStopped();
}