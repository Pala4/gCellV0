#include "ccsvin.h"

#include <QDebug>

#include "../../../scheme/cargument.h"
#include "../../../scheme/cresult.h"

void CCSVIn::proced(const int &timeFrame)
{
	qDebug() << "lb:" << "tf =" << timeFrame << arg1->portalData(timeFrame);
	res1->setPortalData(timeFrame, arg1->portalData(timeFrame) + 1.0);
	res2->setPortalData(timeFrame, arg1->portalData(timeFrame) * 2.0);
}

CCSVIn::CCSVIn(QGraphicsItem *parent) : CDataSource(parent)
{
    setObjectName(QStringLiteral("CCSVIn"));

	arg1 = addArgument("X1");

	res1 = addResult("Y1");
	res2 = addResult("Y2");
}