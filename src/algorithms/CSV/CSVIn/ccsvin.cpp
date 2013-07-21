#include "ccsvin.h"

#include "../../../scheme/cargument.h"
#include "../../../scheme/cresult.h"

CCSVIn::CCSVIn(QGraphicsItem *parent) : CAlgorithm(parent)
{
    setObjectName(QStringLiteral("CCSVIn"));

	addArgument("X1");
	addArgument("X2");
	addArgument("X3");

	addResult("Y1");
	addResult("Y2");
	addResult("Y3");
	addResult("Y4");

	CArgument *Z = addArgument("Z");
	Z->setPortalOrientation(CPortal::Top);
}