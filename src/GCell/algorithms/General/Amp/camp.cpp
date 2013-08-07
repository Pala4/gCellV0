#include "camp.h"

#include "../../../scheme/portal/cargument.h"
#include "../../../scheme/portal/cresult.h"

void CAmp::proced(const int &timeFrame)
{
    res->addBufferData(timeFrame, arg->bufferData(timeFrame)*0.01);
}

CAmp::CAmp(QGraphicsItem *parent) : CDataTransmitter(parent)
{
    setObjectName(QStringLiteral("CAmp"));

	arg = addArgument("X");
	res = addResult("Y");
}