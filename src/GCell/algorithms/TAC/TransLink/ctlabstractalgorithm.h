#ifndef CTLABSTRACTALGORITHM_H
#define CTLABSTRACTALGORITHM_H

#include <QVector>

#include "../../../scheme/timeframe.h"
#include "../../../scheme/databuffer/cdatabuffer.h"

class CTLAbstractAlgorithm
{
private:
	QString m_methodName;
	bool m_prepared;
	QVector<qreal> *m_nomFactors;
	QVector<qreal> *m_denomFactors;

	void init(void);
protected:
	virtual void prepare(const qreal &startTime, const qreal &timeStep, const qreal &endTime);
	virtual stData doCalc(const stTimeLine &timeLine, const QVector<stData> &x, const QVector<stData> &y) = 0;
public:
	CTLAbstractAlgorithm(void);
	CTLAbstractAlgorithm(const QString &methodName);
	CTLAbstractAlgorithm(const QString &methodName, QVector<qreal> *nomFactors, QVector<qreal> *denomFactors);
	virtual ~CTLAbstractAlgorithm(void){}

	const QString& methodName(void) const{return m_methodName;}
	void setMethodName(const QString &methodName){m_methodName = methodName;}
	QVector<qreal>* nomFactors(void){return m_nomFactors;}
	void setNomFactors(QVector<qreal> *nomFactors){m_nomFactors = nomFactors;}
	QVector<qreal>* denomFactors(void){return m_denomFactors;}
	void setDenomFactors(QVector<qreal> *denomFactors){m_denomFactors = denomFactors;}

	void update(const qreal &startTime, const qreal &timeStep, const qreal &endTime);
	stData calculate(const stTimeLine &timeLine,
					 const QVector<stData> &x, const QVector<stData> &y);
};

#endif // CTLABSTRACTALGORITHM_H