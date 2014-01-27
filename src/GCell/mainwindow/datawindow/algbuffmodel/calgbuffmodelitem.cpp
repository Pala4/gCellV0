#include "calgbuffmodelitem.h"

#include "algorithm/calgorithm.h"
#include "portal/cportal.h"
#include "cscheme.h"

void CAlgBuffModelItem::initCAlgBuffModelItem(void)
{
    m_scheme = nullptr;
    m_alg = nullptr;
    m_portal = nullptr;
	m_itemType = CAlgBuffModelItem::NoType;

	setEditable(false);
	setSelectable(false);
}

void CAlgBuffModelItem::addRow(CAlgBuffModelItem *item)
{
    if (item == nullptr)
        return;
    connect(item, SIGNAL(itemChanged(QStandardItem*,int)),
            this, SIGNAL(itemChanged(QStandardItem*,int)));
    appendRow(item);
}

CAlgBuffModelItem::CAlgBuffModelItem(CScheme *scheme, QObject *parent) :
    QObject(parent), QStandardItem()
{
    initCAlgBuffModelItem();

    m_scheme = scheme;
    m_itemType = CAlgBuffModelItem::Scheme;

    if (m_scheme != nullptr) {
        setText(tr("Scheme [%1]").arg(m_scheme->fileName()));
        connect(m_scheme, SIGNAL(destroyed()), this, SLOT(deleteLater()));

        foreach (CAlgorithm *alg, m_scheme->algorithms()) {
            if (alg == nullptr)
                continue;

            CAlgBuffModelItem *algItem = new CAlgBuffModelItem(alg, this);
            addRow(algItem);
        }
    }
}

CAlgBuffModelItem::CAlgBuffModelItem(CAlgorithm *alg, QObject *parent) :
    QObject(parent), QStandardItem()
{
	initCAlgBuffModelItem();

	m_alg = alg;
	m_itemType = CAlgBuffModelItem::Algorithm;

    if (m_alg != nullptr) {
		connect(m_alg, SIGNAL(destroyed()), this, SLOT(deleteLater()));
		setText(m_alg->caption());

        if (!m_alg->argPortals().isEmpty()) {
            CAlgBuffModelItem *argNodeItem = new CAlgBuffModelItem(m_alg->argPortals(),
                                                                   tr("Arguments"), this);
			addRow(argNodeItem);
		}
        if (!m_alg->resPortals().isEmpty()) {
            CAlgBuffModelItem *resNodeItem = new CAlgBuffModelItem(m_alg->resPortals(),
                                                                   tr("Results"), this);
			addRow(resNodeItem);
		}
	}
}

CAlgBuffModelItem::CAlgBuffModelItem(const QList<CPortal*> &portals, const QString &groupName,
                                     QObject *parent) :
    QObject(parent), QStandardItem(groupName)
{
	initCAlgBuffModelItem();

	setText(groupName.isEmpty() ? tr("Portal node") : groupName);
	m_itemType = CAlgBuffModelItem::PortalNodeName;

    foreach (CPortal *portal, portals) {
        if (portal != nullptr) {
			CAlgBuffModelItem *portalItem = new CAlgBuffModelItem(portal, this);
			addRow(portalItem);
		}
	}
}

CAlgBuffModelItem::CAlgBuffModelItem(CPortal *portal, QObject *parent) :
    QObject(parent), QStandardItem()
{
	initCAlgBuffModelItem();
	m_portal = portal;
	m_itemType = CAlgBuffModelItem::Portal;

    if (m_portal != nullptr) {
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
