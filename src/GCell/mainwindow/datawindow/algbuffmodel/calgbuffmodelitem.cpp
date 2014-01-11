#include "calgbuffmodelitem.h"

#include "../../../scheme/algorithm/calgorithm.h"
#include "../../../scheme/portal/cportal.h"

void CAlgBuffModelItem::initCAlgBuffModelItem(void)
{
	m_alg = 0;
	m_portal = 0;
	m_itemType = CAlgBuffModelItem::NoType;

	setEditable(false);
	setSelectable(false);
}

void CAlgBuffModelItem::addRow(CAlgBuffModelItem *item)
{
	if(!item) return;
	connect(item, SIGNAL(itemChanged(QStandardItem*,int)), this, SIGNAL(itemChanged(QStandardItem*,int)));
	appendRow(item);
}

CAlgBuffModelItem::CAlgBuffModelItem(CAlgorithm *alg, QObject *parent) : QObject(parent), QStandardItem()
{
	initCAlgBuffModelItem();

	m_alg = alg;
	m_itemType = CAlgBuffModelItem::Algorithm;

	if(m_alg)
	{
		connect(m_alg, SIGNAL(destroyed()), this, SLOT(deleteLater()));
		setText(m_alg->caption());

		if(!m_alg->argPortals().isEmpty())
		{
			CAlgBuffModelItem *argNodeItem = new CAlgBuffModelItem(m_alg->argPortals(), tr("Arguments"), this);
			addRow(argNodeItem);
		}
		if(!m_alg->resPortals().isEmpty())
		{
			CAlgBuffModelItem *resNodeItem = new CAlgBuffModelItem(m_alg->resPortals(), tr("Results"), this);
			addRow(resNodeItem);
		}
	}
}

CAlgBuffModelItem::CAlgBuffModelItem(const QList<CPortal*> &portals,
									 const QString &groupName,
									 QObject *parent) : QObject(parent), QStandardItem(groupName)
{
	initCAlgBuffModelItem();

	setText(groupName.isEmpty() ? tr("Portal node") : groupName);
	m_itemType = CAlgBuffModelItem::PortalNodeName;

	foreach(CPortal *portal, portals)
	{
		if(portal)
		{
			CAlgBuffModelItem *portalItem = new CAlgBuffModelItem(portal, this);
			addRow(portalItem);
		}
	}
}

CAlgBuffModelItem::CAlgBuffModelItem(CPortal *portal, QObject *parent) : QObject(parent), QStandardItem()
{
	initCAlgBuffModelItem();
	m_portal = portal;
	m_itemType = CAlgBuffModelItem::Portal;

	if(m_portal)
	{
		setText(m_portal->caption());
		setCheckable(true);
		setCheckState(Qt::Checked);
	}
}

void CAlgBuffModelItem::setData(const QVariant &value, int role)
{
	QStandardItem::setData(value, role);
	emit itemChanged(this, role);
}