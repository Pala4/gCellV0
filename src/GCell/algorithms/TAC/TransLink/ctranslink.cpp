#include "ctranslink.h"

#include "../../../scheme/portal/cargument.h"
#include "../../../scheme/portal/cresult.h"

void CTransLink::proced(const int &timeFrame)
{
	m_outRes->appendBufferData(timeFrame, m_inArg->bufferData(timeFrame).value);
}

CTransLink::CTransLink(QGraphicsItem *parent) : CDataTransmitter(parent)
{
	setObjectName(QStringLiteral("CTransLink"));

	m_inArg = 0;
	m_outRes = 0;

	m_inArg = addArgument(tr("In"));
	m_outRes = addResult(tr("Out"));
}