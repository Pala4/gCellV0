#include "camp.h"

#include "../../../scheme/cargument.h"
#include "../../../scheme/cresult.h"

void CAmp::proced(const int &timeFrame)
{
	res->setPortalData(timeFrame, arg->portalData(timeFrame)*2.0);
}

CAmp::CAmp(QGraphicsItem *parent) : CDataTransmitter(parent)
{
    setObjectName(QStringLiteral("CAmp"));

	arg = addArgument("X");

	res = addResult("Y");
}