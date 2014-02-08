#include "ctlabstractalgorithm.h"

#include "ctranslink.h"

void CTLAbstractAlgorithm::init()
{
	m_prepared = false;
    m_nomFactors = nullptr;
    m_denomFactors = nullptr;
}

void CTLAbstractAlgorithm::prepare(const long double &ldblStartTime,
                                   const long double &ldblTimeStep, const long double &ldblEndTime)
{
    Q_UNUSED(ldblStartTime)
    Q_UNUSED(ldblTimeStep)
    Q_UNUSED(ldblEndTime)

	m_prepared = true;
}

CTLAbstractAlgorithm::CTLAbstractAlgorithm()
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

void CTLAbstractAlgorithm::update(const long double &ldblStartTime, const long double &ldblTimeStep,
                                  const long double &ldblEndTime)
{
    prepare(ldblStartTime, ldblTimeStep, ldblEndTime);
}

stData CTLAbstractAlgorithm::calculate(const unsigned long long &ullTFIndex,
                                       const long double &ldblTimeFrame,
                                       const long double &ldblStartTime,
                                       const long double &ldblTimeStep,
                                       const long double &ldblEndTime,
                                       const QVector<stData> &x, const QVector<stData> &y)
{
    if (!m_prepared)
        prepare(ldblStartTime, ldblTimeStep, ldblEndTime);

    return doCalc(ullTFIndex, ldblTimeFrame, x, y);
}