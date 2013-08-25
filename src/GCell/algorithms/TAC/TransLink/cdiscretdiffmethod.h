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
	QVector<qreal> calcZFactors(const QVector<qreal> *factors, const qreal &timeStep);
	qreal calcZSum(const stTimeFrame &frame, const QVector<stData> &data, const QVector<qreal> &zFactors, const quint64 &startIndex = 0);
	stData calcRecurrenceEquation(const stTimeFrame &frame,
								  const QVector<stData> &x, const QVector<qreal> &zNomFactors,
								  const QVector<stData> &y, const QVector<qreal> &zDenomFactors);
protected:
	virtual void prepare(const qreal &startTime, const qreal &timeStep, const qreal &endTime);
	virtual stData doCalc(const stTimeLine &timeLine, const QVector<stData> &x, const QVector<stData> &y);
public:
	CDiscretDiffMethod(void);
	CDiscretDiffMethod(const QString &methodName);
	CDiscretDiffMethod(const QString &methodName, QVector<qreal> *nomFactors, QVector<qreal> *denomFactors);
};

#endif // CDISCRETDIFFMETHOD_H