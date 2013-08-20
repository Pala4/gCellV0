#include "cstepexcitation.h"

#include "../../../scheme/portal/cresult.h"

void CStepExcitation::proced(const int &timeFrame)
{
	m_step->appendBufferData(timeFrame, 1.0);
}

CStepExcitation::CStepExcitation(QGraphicsItem *parent) : CDataSource(parent)
{
	setObjectName(QStringLiteral("CStepExcitation"));

	m_step = 0;

	m_step = addResult(QStringLiteral("Out"));
}