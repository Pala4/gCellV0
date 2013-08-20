#include "ccsvout.h"

#include "../../../scheme/portal/cargument.h"
#include <QDebug>
void CCSVOut::proced(const int &timeFrame)
{
	stData d = arg->bufferData(timeFrame);
	qDebug() << name() << d.value;
}

CCSVOut::CCSVOut(QGraphicsItem *parent) : CDataReceiver(parent)
{
    setObjectName(QStringLiteral("CCSVOut"));

	arg = addArgument("X");
}