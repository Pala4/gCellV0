#include "cstepexcitation.h"

#include "../../../scheme/portal/cresult.h"

void CStepExcitation::proced(const stTimeLine &timeLine)
{
	m_step->appendBuffer(timeLine.timeFrame, 1.0);
}

CStepExcitation::CStepExcitation(QGraphicsItem *parent) : CDataSource(parent)
{
	setObjectName(QStringLiteral("CStepExcitation"));

	m_step = 0;

	m_step = addResult(QStringLiteral("Out"));
}