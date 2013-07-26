#include "ccsvout.h"

#include "../../../scheme/cargument.h"
#include <QDebug>
void CCSVOut::proced(const int &timeFrame)
{
	qreal d = arg->portalData(timeFrame);
	qDebug() << name() << d;
}

CCSVOut::CCSVOut(QGraphicsItem *parent) : CDataReceiver(parent)
{
    setObjectName(QStringLiteral("CCSVOut"));

	arg = addArgument("X");
}