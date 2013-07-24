#include "cresult.h"

#include "clink.h"

CResult::CResult(QGraphicsItem *parent) : CPortal(parent)
{
	setObjectName(QStringLiteral("CResult"));
	setDefaultName(tr("result"));
}

void CResult::calc(const int &timeFrame)
{
	foreach(CLink *link, links())
	{
		if(link)
		{
			link->calc(timeFrame);
		}
	}
}