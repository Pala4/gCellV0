#include "ccsvin.h"

#include <qmath.h>

#include "../../../scheme/cargument.h"
#include "../../../scheme/cresult.h"

void CCSVIn::proced(const int &timeFrame)
{
	res1->setPortalData(timeFrame, qSin(timeFrame) + arg1->portalData(timeFrame) + 1.0);
	res2->setPortalData(timeFrame, qSin(timeFrame) + arg1->portalData(timeFrame) * 2.0);
}

CCSVIn::CCSVIn(QGraphicsItem *parent) : CDataSource(parent)
{
    setObjectName(QStringLiteral("CCSVIn"));

	arg1 = addArgument("X1");

	res1 = addResult("Y1");
	res2 = addResult("Y2");
}