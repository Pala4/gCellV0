#include "calgbuffmodelitem.h"

#include "algorithm/calgorithm.h"
#include "portal/cportal.h"
#include "portal/cargument.h"
#include "portal/cresult.h"
#include "cscheme.h"

void CAlgBuffModelItem::setItemType(const CAlgBuffModelItem::AlgBuffModelItemType &itemType)
{
    m_itemType = itemType;
}

CAlgBuffModelItem::CAlgBuffModelItem(QObject *parent) : QObject(parent), QStandardItem()
{
    m_itemType = CAlgBuffModelItem::NoType;

    setEditable(false);
    setSelectable(false);
}

void CAlgBuffModelItem::setData(const QVariant &value, int role)
{
	QStandardItem::setData(value, role);
	emit itemChanged(this, role);
}

void CAlgBuffModelItem::addRow(CAlgBuffModelItem *item)
{
    if (item == nullptr)
        return;
    connect(item, SIGNAL(itemChanged(QStandardItem*,int)),
            this, SIGNAL(itemChanged(QStandardItem*,int)));
    appendRow(item);
}

void CAlgBuffModelItem::clear()
{
    removeRows(0, rowCount());
}

void CAlgBuffModelItem::changeText(const QString &newText)
{
    setText(newText);
}

/*!
 * \class CGroupItem
 */
CGroupItem::CGroupItem(const QString &name, QObject *parent) : CAlgBuffModelItem(parent)
{
    setItemType(CAlgBuffModelItem::PortalNodeName);

    m_name = name.isEmpty() ? tr("Portal node") : name;

    setText(m_name);
}

/*!
 * \class CSchemeItem
 */
bool CSchemeItem::contains(CAlgorithm *alg)
{
    return (indexOf(alg) > -1);
}

int CSchemeItem::indexOf(CAlgorithm *alg)
{
    for (int rowIndex = 0; rowIndex < rowCount(); ++rowIndex) {
        CAlgItem *algItem = dynamic_cast<CAlgItem*>(child(rowIndex));
        if ((algItem != nullptr)
                && (algItem->alg() != nullptr)
                && (algItem->alg() == alg)) {
            return rowIndex;
        }
    }

    return -1;
}

CSchemeItem::CSchemeItem(CScheme *scheme, QObject *parent) : CAlgBuffModelItem(parent)
{
    m_scheme = nullptr;

    setItemType(CAlgBuffModelItem::Scheme);

    m_scheme = scheme;
    if (m_scheme != nullptr) {
        setText(tr("Scheme [%1]").arg(m_scheme->fileName()));
        connect(m_scheme, SIGNAL(destroyed()), this, SLOT(deleteLater()));
        connect(m_scheme, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));
        connect(m_scheme, SIGNAL(fileNameChanged(QString)), this, SLOT(changeText(QString)));

        addAlgs(m_scheme->selectedAlgorithms());
    }
}

void CSchemeItem::addAlgs(const QList<CAlgorithm *> &algs)
{
    remAlgs();

    foreach (CAlgorithm *alg, algs)
        addAlg(alg);
}

void CSchemeItem::addAlg(CAlgorithm *alg)
{
    if ((alg != nullptr) && (!contains(alg)))
        addRow(new CAlgItem(alg, this));
}

void CSchemeItem::remAlgs(const QList<CAlgorithm*> &algs)
{
    if (algs.isEmpty())
        clear();

    foreach (CAlgorithm *alg, algs)
        remAlg(alg);
}

void CSchemeItem::remAlg(CAlgorithm *alg)
{
    if ((alg == nullptr) || (!contains(alg)))
        return;

    int algIndex = indexOf(alg);
    if (algIndex > -1)
        removeRow(algIndex);
}

void CSchemeItem::onSelectionChanged()
{
    if (m_scheme == nullptr)
        return;

    foreach (CAlgorithm *alg, m_scheme->algorithms()) {
        if (alg == nullptr)
            continue;

        if (alg->isSelected())
            addAlg(alg);
        else
            remAlg(alg);
    }
}

/*!
 * \class CAlgItem
 */
bool CAlgItem::contains(CGroupItem *group, CPortal *portal)
{
    return (indexOf(group, portal) > -1);
}

int CAlgItem::indexOf(CGroupItem *group, CPortal *portal)
{
    if (group == nullptr)
        return -1;

    for (int rowIndex = 0; rowIndex < group->rowCount(); ++rowIndex) {
        CPortalItem *portalItem = dynamic_cast<CPortalItem*>(group->child(rowIndex));
        if ((portalItem != nullptr)
                && (portalItem->portal() != nullptr)
                && (portalItem->portal() == portal)) {
            return rowIndex;
        }
    }

    return -1;
}

CAlgItem::CAlgItem(CAlgorithm *alg, QObject *parent) : CAlgBuffModelItem(parent)
{
    m_alg = nullptr;
    m_argGroup = nullptr;
    m_resGroup = nullptr;

    setItemType(CAlgBuffModelItem::Algorithm);

    m_alg = alg;
    if (m_alg != nullptr) {
        setText(m_alg->caption());
        connect(m_alg, SIGNAL(destroyed()), this, SLOT(deleteLater()));
        connect(m_alg, SIGNAL(portalAdded(CPortal*)), this, SLOT(onPortalAdded(CPortal*)));
        connect(m_alg, SIGNAL(nameChanged(QString)), this, SLOT(changeText(QString)));

        if (!m_alg->argPortals().isEmpty()) {
            m_argGroup = new CGroupItem(tr("Arguments"), this);
            addRow(m_argGroup);
            addPortals(m_argGroup, m_alg->argPortals());
        }
        if (!m_alg->resPortals().isEmpty()) {
            m_resGroup = new CGroupItem(tr("Results"), this);
            addRow(m_resGroup);
            addPortals(m_resGroup, m_alg->resPortals());
        }
    }
}

void CAlgItem::addPortals(CGroupItem *group, const QList<CPortal*> &portals)
{
    if (group != nullptr) {
        foreach (CPortal *arg, portals)
            addPortal(group, arg);
    }
}

void CAlgItem::addPortal(CGroupItem *group, CPortal *portal)
{
    if ((group == nullptr) || (portal == nullptr) || contains(group, portal))
        return;

    group->addRow(new CPortalItem(portal, group));
}

void CAlgItem::remPortals(CGroupItem *group, const QList<CPortal*> &portals)
{
    if (group == nullptr)
        return;

    if (portals.isEmpty())
        group->clear();

    foreach (CPortal *arg, portals)
        remPortal(group, arg);
}

void CAlgItem::remPortal(CGroupItem *group, CPortal *portal)
{
    if ((group == nullptr) || (portal == nullptr) || !contains(group, portal))
        return;

    int portalIndex = indexOf(group, portal);
    if (portalIndex > -1)
        group->removeRow(portalIndex);
}

void CAlgItem::onPortalAdded(CPortal *portal)
{
    CGroupItem *group = nullptr;
    if (qobject_cast<CArgument*>(portal))
        group = m_argGroup;
    else if (qobject_cast<CResult*>(portal))
        group = m_resGroup;

    addPortal(group, portal);
}

/*!
 * \class CPortalItem
 */
CPortalItem::CPortalItem(CPortal *portal, QObject *parent) : CAlgBuffModelItem(parent)
{
    setItemType(CAlgBuffModelItem::Portal);

    m_portal = portal;

    if (m_portal != nullptr) {
        connect(m_portal, SIGNAL(destroyed()), this, SLOT(deleteLater()));
        connect(m_portal, SIGNAL(nameChanged(QString)), this, SLOT(changeText(QString)));

        setText(m_portal->caption());
        setCheckable(true);
        setCheckState(Qt::Checked);
    }
}