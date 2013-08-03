#include "ccsvin.h"

#include <qmath.h>

#include "../../../scheme/portal/cargument.h"
#include "../../../scheme/portal/cresult.h"

void CCSVIn::proced(const int &timeFrame)
{
	res1->addBufferData(timeFrame, qSin(timeFrame) + arg1->bufferData(timeFrame) + 1.0);
	res2->addBufferData(timeFrame, qSin(timeFrame) + arg1->bufferData(timeFrame) * 2.0);
}

CCSVIn::CCSVIn(QGraphicsItem *parent) : CDataSource(parent)
{
    setObjectName(QStringLiteral("CCSVIn"));

	arg1 = addArgument("X1");
	addArgument("X2");
	addArgument("X3");
	addArgument("X4");

	res1 = addResult("Y1");
	res2 = addResult("Y2");
	addResult("Y3");
	addResult("Y4");
	addResult("Y5");
	addResult("Y6");
}