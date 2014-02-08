#ifndef CENGINE_H
#define CENGINE_H

#include <vector>

class CScheme;
class CAlgorithm;
class CDataSource;
class CPortal;
class CResult;
class CArgument;
class CLink;

class CTraceData
{
private:
    std::vector<CDataSource*> m_dataSources;
    std::vector<CAlgorithm*> m_inspectedAlgorithms;
    std::vector<CPortal*> m_loopBackPortals;
public:
    CTraceData(){}

    void setDataSources(const std::vector<CDataSource*> &dataSources);
    const std::vector<CDataSource*>& dataSources() const{return m_dataSources;}
    void clearDataSources(){m_dataSources.clear();}

    bool isInspected(CAlgorithm *algorithm);
	void addInspectedAlgorithm(CAlgorithm *algorithm);
	void removeInspectedAlgorithm(CAlgorithm *algorithm);
    void clearInspectedAlgorithm(){m_inspectedAlgorithms.clear();}

    bool isLoopBackPortal(CPortal *portal);
	void addLoopBackPortal(CPortal *portal);
    void clearLoopBackPortals(){m_loopBackPortals.clear();}

    void release();
};

class CEngine
{
private:
	CTraceData m_traceData;

	void traceArgument(CArgument *argument);
	void traceLink(CLink *link);
	void traceResult(CResult *result);
	void traceAlgorithm(CAlgorithm *algorithm);
    void traceScheme(CScheme *scheme);
public:
    explicit CEngine(){}

    void calc(CScheme *scheme);
};

#endif // CENGINE_H