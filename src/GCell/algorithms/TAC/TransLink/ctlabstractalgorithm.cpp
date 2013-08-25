#include "ctlabstractalgorithm.h"

#include "ctranslink.h"

void CTLAbstractAlgorithm::init(void)
{
	m_prepared = false;
	m_nomFactors = 0;
	m_denomFactors = 0;
}

void CTLAbstractAlgorithm::prepare(const qreal &startTime, const qreal &timeStep, const qreal &endTime)
{
	Q_UNUSED(startTime)
	Q_UNUSED(timeStep)
	Q_UNUSED(endTime)

	m_prepared = true;
}

CTLAbstractAlgorithm::CTLAbstractAlgorithm(void)
{
	init();
}

CTLAbstractAlgorithm::CTLAbstractAlgorithm(const QString &methodName)
{
	init();

	m_methodName = methodName;
}

CTLAbstractAlgorithm::CTLAbstractAlgorithm(const QString &methodName, QVector<qreal> *nomFactors, QVector<qreal> *denomFactors)
{
	m_methodName = methodName;
	setNomFactors(nomFactors);
	setDenomFactors(denomFactors);
}

void CTLAbstractAlgorithm::update(const qreal &startTime, const qreal &timeStep, const qreal &endTime)
{
	prepare(startTime, timeStep, endTime);
}

stData CTLAbstractAlgorithm::calculate(const stTimeLine &timeLine,
									   const QVector<stData> &x, const QVector<stData> &y)
{
	if(!m_prepared) prepare(timeLine.args.startTime, timeLine.args.timeStep, timeLine.args.endTime);
	return doCalc(timeLine, x, y);
}