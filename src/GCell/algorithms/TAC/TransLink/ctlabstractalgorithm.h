#ifndef CTLABSTRACTALGORITHM_H
#define CTLABSTRACTALGORITHM_H

#include <QVector>

#include "databuffer/cdatabuffer.h"

class CTLAbstractAlgorithm
{
private:
	QString m_methodName;
	bool m_prepared;
	QVector<qreal> *m_nomFactors;
	QVector<qreal> *m_denomFactors;

    void init();
protected:
    virtual void prepare(const long double &ldblStartTime, const long double &ldblTimeStep,
                         const long double &ldblEndTime);
    virtual stData doCalc(const unsigned long long &ullTFIndex, const long double &ldblTimeFrame,
                          const QVector<stData> &x, const QVector<stData> &y) = 0;
public:
    CTLAbstractAlgorithm();
	CTLAbstractAlgorithm(const QString &methodName);
    CTLAbstractAlgorithm(const QString &methodName, QVector<qreal> *nomFactors,
                         QVector<qreal> *denomFactors);
	virtual ~CTLAbstractAlgorithm(void){}

    const QString& methodName() const{return m_methodName;}
	void setMethodName(const QString &methodName){m_methodName = methodName;}
    QVector<qreal>* nomFactors(){return m_nomFactors;}
	void setNomFactors(QVector<qreal> *nomFactors){m_nomFactors = nomFactors;}
    QVector<qreal>* denomFactors(){return m_denomFactors;}
	void setDenomFactors(QVector<qreal> *denomFactors){m_denomFactors = denomFactors;}

    void update(const long double &ldblStartTime, const long double &ldblTimeStep,
                const long double &ldblEndTime);
    stData calculate(const unsigned long long &ullTFIndex, const long double &ldblTimeFrame,
                     const long double &ldblStartTime, const long double &ldblTimeStep,
                     const long double &ldblEndTime, const QVector<stData> &x,
                     const QVector<stData> &y);
};

#endif // CTLABSTRACTALGORITHM_H