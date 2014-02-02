#include "cengine.h"

#include <qmath.h>

#include <assert.h>
#include <iostream>

#include "cscheme.h"
#include "algorithm/calgorithm.h"
#include "algorithm/cdatasource.h"
#include "portal/cresult.h"
#include "portal/cargument.h"
#include "link/clink.h"
#include "elementlistutil.h"

/*!
 * \class CTraceData
 */
void CTraceData::setDataSources(const std::vector<CDataSource*> &dataSources)
{
    m_dataSources = dataSources;
}

bool CTraceData::isInspected(CAlgorithm *algorithm)
{
    if (algorithm == nullptr) {
        std::cout << "Warning: argument <algorithm> is null in ["
                  << __FILE__ << ":" << __LINE__ << "]";
        return false;
    }

    return (std::find(m_inspectedAlgorithms.begin(), m_inspectedAlgorithms.end(), algorithm)
            != m_inspectedAlgorithms.end());
}

void CTraceData::addInspectedAlgorithm(CAlgorithm *algorithm)
{
    if (algorithm == nullptr) {
        std::cout << "Warning: argument <algorithm> is null in ["
                  << __FILE__ << ":" << __LINE__ << "]";
        return;
    }

    if (std::find(m_inspectedAlgorithms.begin(), m_inspectedAlgorithms.end(), algorithm)
            != m_inspectedAlgorithms.end()) {
        return;
    }
    m_inspectedAlgorithms.push_back(algorithm);
}

void CTraceData::removeInspectedAlgorithm(CAlgorithm *algorithm)
{
    if (algorithm == nullptr) {
        std::cout << "Warning: argument <algorithm> is null in ["
                  << __FILE__ << ":" << __LINE__ << "]";
        return;
    }

    if (std::find(m_inspectedAlgorithms.begin(), m_inspectedAlgorithms.end(), algorithm)
            == m_inspectedAlgorithms.end()) {
        return;
    }
    int idx = std::find(m_inspectedAlgorithms.begin(), m_inspectedAlgorithms.end(), algorithm)
            - m_inspectedAlgorithms.begin();
    m_inspectedAlgorithms.erase(m_inspectedAlgorithms.begin() + idx);
}

bool CTraceData::isLoopBackPortal(CPortal *portal)
{
    if (portal == nullptr) {
        std::cout << "Warning: argument <portal> is null in ["
                  << __FILE__ << ":" << __LINE__ << "]";
        return false;
    }

    return (std::find(m_loopBackPortals.begin(), m_loopBackPortals.end(), portal)
            != m_loopBackPortals.end());
}

void CTraceData::addLoopBackPortal(CPortal *portal)
{
    if (portal == nullptr) {
        std::cout << "Warning: argument <portal> is null in ["
                  << __FILE__ << ":" << __LINE__ << "]";
        return;
    }

    if (std::find(m_loopBackPortals.begin(), m_loopBackPortals.end(), portal)
            != m_loopBackPortals.end()) {
        return;
    }
    m_loopBackPortals.push_back(portal);
}

void CTraceData::release()
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
    if (argument == nullptr) {
        std::cout << "Warning: argument <argument> is null in ["
                  << __FILE__ << ":" << __LINE__ << "]";
        return;
    }

    if (m_traceData.isLoopBackPortal(argument))
        return;
	CAlgorithm *alg = dynamic_cast<CAlgorithm*>(argument->parentItem());
    if (m_traceData.isInspected(alg)) {
		m_traceData.addLoopBackPortal(argument);
		argument->setLoopBackPortal(true);
		return;
	}
    if (alg != nullptr)
        traceAlgorithm(alg);
}

void CEngine::traceLink(CLink *link)
{
    if (link == nullptr) {
        std::cout << "Warning: argument <link> is null in ["
                  << __FILE__ << ":" << __LINE__ << "]";
        return;
    }

    traceArgument(link->argument());
}

void CEngine::traceResult(CResult *result)
{
    if (result == nullptr) {
        std::cout << "Warning: argument <result> is null in ["
                  << __FILE__ << ":" << __LINE__ << "]";
        return;
    }

    for (CLink *link : result->links().toVector().toStdVector()) {
        if (link != nullptr)
            traceLink(link);
	}
}

void CEngine::traceAlgorithm(CAlgorithm *algorithm)
{
    if (algorithm == nullptr) {
        std::cout << "Warning: argument <algorithm> is null in ["
                  << __FILE__ << ":" << __LINE__ << "]";
        return;
    }

    m_traceData.addInspectedAlgorithm(algorithm);
    for (CResult *res : algorithm->results().toVector().toStdVector()) {
        if (res != nullptr)
            traceResult(res);
	}
	m_traceData.removeInspectedAlgorithm(algorithm);
}

void CEngine::traceScheme(CScheme *scheme)
{
    if (scheme == nullptr) {
        std::cout << "Warning: argument <scheme> is null in ["
                  << __FILE__ << ":" << __LINE__ << "]";
        return;
    }

    std::vector<CDataSource*> dataSources =
            getElements<CDataSource*, QGraphicsItem*>(scheme->items()).toVector().toStdVector();
    m_traceData.setDataSources(dataSources);
    for (CDataSource *ds : m_traceData.dataSources()) {
        if (ds != nullptr) {
            for (CArgument *arg : ds->arguments().toVector().toStdVector()) {
                if (arg == nullptr) {
                    m_traceData.addLoopBackPortal(arg);
                    arg->setLoopBackPortal(true);
                }
            }
        }
    }
    for (CDataSource *ds : m_traceData.dataSources()) {
        if(ds != nullptr)
            traceAlgorithm(ds);
	}
}

void CEngine::calc(CScheme *scheme)
{
    if (scheme == nullptr) {
        std::cout << "Warning: argument <scheme> is null in ["
                  << __FILE__ << ":" << __LINE__ << "]";
        return;
    }

    long double ldblStartTime = 0.0L;
    long double ldblTimeStep = 0.01L;
    long double ldblEndTime = 100.0L;

    for (CElement *element : scheme->elements().toVector().toStdVector()) {
        if (element != nullptr)
            element->beforeCalc(ldblStartTime, ldblTimeStep, ldblEndTime);
    }

    traceScheme(scheme);
    unsigned long long ullTFIndex = 0;
    long double ldblTimeFrame = ldblStartTime;
    while (ldblTimeFrame <= ldblEndTime) {
        for (CDataSource *ds : m_traceData.dataSources()) {
            if (ds != nullptr)
                ds->calc(ullTFIndex, ldblTimeFrame, ldblStartTime, ldblTimeStep, ldblEndTime);
        }

        ++ullTFIndex;
        ldblTimeFrame += ldblTimeStep;
    }
    unsigned long long ullTFCount = (ullTFIndex == 0) ? 0 : ullTFIndex + 1;
	m_traceData.release();

    for (CElement *element : scheme->elements().toVector().toStdVector()) {
        if (element != nullptr)
            element->afterCalc(ullTFCount, ldblTimeFrame, ldblStartTime, ldblTimeStep, ldblEndTime);
    }
}