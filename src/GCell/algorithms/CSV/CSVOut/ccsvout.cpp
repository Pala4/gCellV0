#include "ccsvout.h"

#include "../../../scheme/portal/cargument.h"

void CCSVOut::proced(const stTimeLine &timeLine)
{
	Q_UNUSED(timeLine)
}

CCSVOut::CCSVOut(QGraphicsItem *parent) : CDataReceiver(parent)
{
    setObjectName(QStringLiteral("CCSVOut"));

	arg = addArgument("X");
}