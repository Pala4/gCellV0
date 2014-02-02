#ifndef CDISCRETDIFFMETHOD_H
#define CDISCRETDIFFMETHOD_H

#include "ctlabstractalgorithm.h"

class CDiscretDiffMethod : public CTLAbstractAlgorithm
{
private:
	QVector<qreal> m_zNomFactors;
	QVector<qreal> m_zDenomFactors;

	int factorial(const int &n);
	qreal combination(const int &n, const int &k);
    QVector<qreal> calcZFactors(const QVector<qreal> *factors, const long double &ldblTimeStep);
    qreal calcZSum(const unsigned long long ullTFIndex, const QVector<stData> &data,
                   const QVector<qreal> &zFactors, const quint64 &startIndex = 0);
    stData calcRecurrenceEquation(const unsigned long long &ullTFIndex,
                                  const long double &ldblTimeFrame,
								  const QVector<stData> &x, const QVector<qreal> &zNomFactors,
								  const QVector<stData> &y, const QVector<qreal> &zDenomFactors);
protected:
    void prepare(const long double &ldblStartTime, const long double &ldblTimeStep,
                 const long double &ldblEndTime);
    stData doCalc(const unsigned long long &ullTFIndex, const long double &ldblTimeFrame,
                  const QVector<stData> &x, const QVector<stData> &y);
public:
    CDiscretDiffMethod();
	CDiscretDiffMethod(const QString &methodName);
    CDiscretDiffMethod(const QString &methodName, QVector<qreal> *nomFactors,
                       QVector<qreal> *denomFactors);
};

#endif // CDISCRETDIFFMETHOD_H