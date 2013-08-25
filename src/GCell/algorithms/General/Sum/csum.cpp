#include "csum.h"

#include "../../../scheme/portal/cargument.h"
#include "../../../scheme/portal/cresult.h"

void CSum::proced(const stTimeLine &timeLine)
{
	m_sumRes->appendBuffer(timeLine.timeFrame,
							   m_inArg->bufferData(timeLine.timeFrame.timeFrameIndex).value -
							   m_x1Arg->bufferData(timeLine.timeFrame.timeFrameIndex).value -
							   m_x2Arg->bufferData(timeLine.timeFrame.timeFrameIndex).value);
}

CSum::CSum(QGraphicsItem *parent) : CDataTransmitter(parent)
{
    setObjectName(QStringLiteral("CSum"));

	m_inArg = 0;
	m_x1Arg = 0;
	m_x2Arg = 0;
	m_sumRes = 0;

	m_inArg = addArgument("In");

	m_x1Arg = addArgument("X1");
	m_x1Arg->setPortalOrientation(CPortal::Top);

	m_x2Arg = addArgument("X2");
	m_x2Arg->setPortalOrientation(CPortal::Bottom);

	m_sumRes = addResult("Sum");
}