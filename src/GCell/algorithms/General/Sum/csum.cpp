#include "csum.h"

#include "../../../scheme/portal/cargument.h"
#include "../../../scheme/portal/cresult.h"

void CSum::proced(const int &timeFrame)
{
    m_resSum->addBufferData(timeFrame, m_arg1->bufferData(timeFrame) - m_arg2->bufferData(timeFrame) - m_arg3->bufferData(timeFrame));
}

CSum::CSum(QGraphicsItem *parent) : CDataTransmitter(parent)
{
    setObjectName(QStringLiteral("CSum"));

    m_arg1 = 0;
    m_arg2 = 0;
    m_arg3 = 0;
    m_resSum = 0;

    m_arg1 = addArgument("X1");

    m_arg2 = addArgument("X2");
    m_arg2->setPortalOrientation(CPortal::Top);

    m_arg3 = addArgument("X2");
    m_arg3->setPortalOrientation(CPortal::Bottom);

    m_resSum = addResult("Sum");
}