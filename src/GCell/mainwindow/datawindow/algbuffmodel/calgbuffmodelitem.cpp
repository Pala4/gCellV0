#include "calgbuffmodelitem.h"

#include "../../../scheme/algorithm/calgorithm.h"
#include "../../../scheme/portal/cportal.h"

void CAlgBuffModelItem::initConstructor(void)
{
	m_alg = 0;
	m_portal = 0;
	m_itemType = CAlgBuffModelItem::NoType;
}

CAlgBuffModelItem::CAlgBuffModelItem(CAlgorithm *alg) : QStandardItem()
{
	initConstructor();
	m_alg = alg;
	m_itemType = CAlgBuffModelItem::Algorithm;

	if(m_alg) setText(m_alg->caption());
}

CAlgBuffModelItem::CAlgBuffModelItem(const QString &portalNodeName) : QStandardItem(portalNodeName)
{
	initConstructor();
	m_portalNodeName = portalNodeName;
	m_itemType = CAlgBuffModelItem::PortalNodeName;
}

CAlgBuffModelItem::CAlgBuffModelItem(CPortal *portal) : QStandardItem()
{
	initConstructor();
	m_portal = portal;
	m_itemType = CAlgBuffModelItem::Portal;

	if(m_portal) setText(m_portal->caption());
}