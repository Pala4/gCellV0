#include "cdiscretdiffmethod.h"

#include <qmath.h>

int CDiscretDiffMethod::factorial(const int &n)
{
	if(n > 1)
	{
		int k = 1;
		for(int i = 1; i <= n; ++i) k = i*k;
		return k;
	}

	return 1;
}

qreal CDiscretDiffMethod::combination(const int &n, const int &k)
{
	if(n <= k) return factorial(k)/(factorial(n)*factorial(k - n));

	return 1.0;
}

QVector<qreal> CDiscretDiffMethod::calcZFactors(const QVector<qreal> *factors, const qreal &timeStep)
{
	QVector<qreal> resZFactors;
	if(!factors) return resZFactors;
	if(timeStep == 0.0) return resZFactors;
	resZFactors.resize(factors->count());
	resZFactors.fill(0.0);
	qreal polyPower = factors->count() - 1;
	for(int i = 0; i < factors->count(); i++)
	{
		resZFactors[polyPower - i] = 0.0;
		for(int j = 0; j <= i; j++)
		{
			resZFactors[polyPower - i] += qPow(-1.0, polyPower - i)*factors->at(polyPower - j)/
										  qPow(timeStep, polyPower - j)*combination(i - j, polyPower - j);
		}
	}
	return resZFactors;
}

qreal CDiscretDiffMethod::calcZSum(const stTimeFrame &frame, const QVector<stData> &data, const QVector<qreal> &zFactors, const quint64 &startIndex)
{
	qreal zSum = 0.0;
	if(startIndex >= (quint64)zFactors.count()) return zSum;
	for(quint64 zFactorIndex = startIndex; zFactorIndex < (quint64)zFactors.count(); ++zFactorIndex)
	{
		if(frame.timeFrameIndex - zFactorIndex < (quint64)data.count())
		{
			zSum += data.at(frame.timeFrameIndex - zFactorIndex).value*zFactors.at(zFactorIndex);
		}
	}
	return zSum;
}

stData CDiscretDiffMethod::calcRecurrenceEquation(const stTimeFrame &frame,
												  const QVector<stData> &x, const QVector<qreal> &zNomFactors,
												  const QVector<stData> &y, const QVector<qreal> &zDenomFactors)
{
	stData y0(frame, 0.0);

	qreal sumZA = calcZSum(frame, x, zNomFactors);
	qreal sumZB = calcZSum(frame, y, zDenomFactors, 1);

	y0.value = (!zDenomFactors.isEmpty() && zDenomFactors.at(0) != 0.0) ? (sumZA - sumZB)/zDenomFactors.at(0) : 0.0;

	return y0;
}

void CDiscretDiffMethod::prepare(const qreal &startTime, const qreal &timeStep, const qreal &endTime)
{
	Q_UNUSED(startTime)
	Q_UNUSED(endTime)

	m_zNomFactors = calcZFactors(nomFactors(), timeStep);
	m_zDenomFactors = calcZFactors(denomFactors(), timeStep);
}

stData CDiscretDiffMethod::doCalc(const stTimeLine &timeLine, const QVector<stData> &x, const QVector<stData> &y)
{
	return calcRecurrenceEquation(timeLine.timeFrame, x, m_zNomFactors, y, m_zDenomFactors);
}

CDiscretDiffMethod::CDiscretDiffMethod(void) : CTLAbstractAlgorithm()
{
}

CDiscretDiffMethod::CDiscretDiffMethod(const QString &methodName) : CTLAbstractAlgorithm(methodName)
{
}

CDiscretDiffMethod::CDiscretDiffMethod(const QString &methodName, QVector<qreal> *nomFactors, QVector<qreal> *denomFactors) : CTLAbstractAlgorithm(methodName, nomFactors, denomFactors)
{
}