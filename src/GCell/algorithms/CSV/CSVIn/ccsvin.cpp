#include "ccsvin.h"

#include <qmath.h>

#include "../../../scheme/portal/cargument.h"
#include "../../../scheme/portal/cresult.h"

void CCSVIn::proced(const int &timeFrame)
{
    m_res1->addBufferData(timeFrame, qSin(timeFrame));
    m_res2->addBufferData(timeFrame, qSin(timeFrame*2.0));
}

CCSVIn::CCSVIn(QGraphicsItem *parent) : CDataSource(parent)
{
    setObjectName(QStringLiteral("CCSVIn"));

    m_res1 = addResult("Y1");
    m_res2 = addResult("Y2");
}