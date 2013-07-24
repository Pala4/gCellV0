#ifndef CENGINE_H
#define CENGINE_H

#include <QObject>

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
	QList<CDataSource*> m_dataSources;
	QList<CAlgorithm*> m_inspectedAlgorithms;
	QList<CPortal*> m_loopBackPortals;
public:
	CTraceData(void){}

	void setDataSources(const QList<CDataSource*> &dataSources){m_dataSources.clear(); m_dataSources = dataSources;}
	const QList<CDataSource*>& dataSources(void) const{return m_dataSources;}
	void clearDataSources(void){m_dataSources.clear();}

	bool isInspected(CAlgorithm *algorithm){if(!algorithm) return false; return m_inspectedAlgorithms.contains(algorithm);}
	void addInspectedAlgorithm(CAlgorithm *algorithm);
	void removeInspectedAlgorithm(CAlgorithm *algorithm);
	void clearInspectedAlgorithm(void){m_inspectedAlgorithms.clear();}

	bool isLoopBackPortal(CPortal *portal){if(!portal) return false; return m_loopBackPortals.contains(portal);}
	void addLoopBackPortal(CPortal *portal);
	void clearLoopBackPortals(void){m_loopBackPortals.clear();}

	void release(void);
};

class CEngine : public QObject
{
    Q_OBJECT
private:
	CTraceData m_traceData;
    CScheme *m_scheme;

	void traceArgument(CArgument *argument);
	void traceLink(CLink *link);
	void traceResult(CResult *result);
	void traceAlgorithm(CAlgorithm *algorithm);
	void traceScheme(void);
public:
    explicit CEngine(QObject *parent = 0);

    CScheme* scheme(void){return m_scheme;}
    void setScheme(CScheme *scheme);
private slots:
    void onSchemeDestroyed(void);
public slots:
    void calc(void);
};

#endif // CENGINE_H