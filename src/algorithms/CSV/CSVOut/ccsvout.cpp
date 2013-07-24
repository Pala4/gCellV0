#include "ccsvout.h"

#include "../../../scheme/cargument.h"

void CCSVOut::proced(const int &timeFrame)
{
	qreal d = arg->portalData(timeFrame);
	d = 0.0;
}

CCSVOut::CCSVOut(QGraphicsItem *parent) : CDataReceiver(parent)
{
    setObjectName(QStringLiteral("CCSVOut"));

	arg = addArgument("X");
}