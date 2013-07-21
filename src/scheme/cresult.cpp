#include "cresult.h"

CResult::CResult(QGraphicsItem *parent) : CPortal(parent)
{
	setObjectName(QStringLiteral("CResult"));
	setDefaultName(tr("result"));
}