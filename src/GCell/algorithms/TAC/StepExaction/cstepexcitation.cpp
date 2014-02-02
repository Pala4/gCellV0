#include "cstepexcitation.h"

#include "../../../scheme/portal/cresult.h"

void CStepExcitation::proced(const unsigned long long &ullTFIndex, const long double &ldblTimeFrame,
                             const long double &ldblStartTime, const long double &ldblTimeStep,
                             const long double &ldblEndTime)
{
    Q_UNUSED(ullTFIndex)
    Q_UNUSED(ldblStartTime)
    Q_UNUSED(ldblTimeStep)
    Q_UNUSED(ldblEndTime)

    m_pResStep->appendBuffer(ldblTimeFrame, 1.0L);
}

CStepExcitation::CStepExcitation(QGraphicsItem *parent) : CDataSource(parent)
{
	setObjectName(QStringLiteral("CStepExcitation"));

    m_pResStep = addResult(tr("Step"));
}