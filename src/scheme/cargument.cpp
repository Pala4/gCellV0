#include "cargument.h"

#include "calgorithm.h"

CArgument::CArgument(QGraphicsItem *parent) : CPortal(parent)
{
	setObjectName(QStringLiteral("CArgument"));
	setDefaultName(tr("argument"));
}

void CArgument::calc(const int &timeFrame)
{
	if(isLoopBackPortal()) return;
	CAlgorithm *alg = dynamic_cast<CAlgorithm*>(parentItem());
	if(alg) alg->calc(timeFrame);
}