#include "calgtreemodel.h"

#include "calgbuffmodelitem.h"
#include "algorithm/calgorithm.h"
#include "cscheme.h"

QList<CPortal*> CAlgTreeModel::getCheckedPortalsRecurrence(QStandardItem *item)
{
    QList<CPortal*> checkedPortals;
    CAlgBuffModelItem *algBItem = dynamic_cast<CAlgBuffModelItem*>(item);
    if (algBItem == nullptr)
        return checkedPortals;

    if (algBItem->type() == CAlgBuffModelItem::Portal) {
        CPortalItem *portalItem = qobject_cast<CPortalItem*>(algBItem);
        if ((portalItem != nullptr)
                && (portalItem->portal() != nullptr)
                && (portalItem->checkState() == Qt::Checked)) {
            checkedPortals << portalItem->portal();
        }
    } else {
        for (int childIndex = 0; childIndex < algBItem->rowCount(); ++childIndex)
            checkedPortals << getCheckedPortalsRecurrence(algBItem->child(childIndex));
    }

    return checkedPortals;
}

bool CAlgTreeModel::isContains(CScheme *scheme)
{
    for (int row = 0; row < rowCount(); ++row) {
        CSchemeItem *schemeItem = dynamic_cast<CSchemeItem*>(item(row));
        if ((schemeItem != nullptr)
                && (schemeItem->scheme() != nullptr)
                && (schemeItem->scheme() == scheme)) {
            return true;
        }
    }
    return false;
}

CAlgTreeModel::CAlgTreeModel(QObject *parent) : QStandardItemModel(parent)
{
    setObjectName(QStringLiteral("CAlgBuffModel"));
}

QList<CPortal*> CAlgTreeModel::checkedPortalList(void)
{
    QList<CPortal*> checkedPortals;
    for (int rowIndex = 0; rowIndex < rowCount(); ++rowIndex)
        checkedPortals << getCheckedPortalsRecurrence(item(rowIndex));

    return checkedPortals;
}

void CAlgTreeModel::onItemChanged(QStandardItem *item, const int &role)
{
    CAlgBuffModelItem *algBItem = dynamic_cast<CAlgBuffModelItem*>(item);
    if(algBItem == nullptr)
        return;

    switch (role) {
    case Qt::CheckStateRole:
    {
        CPortalItem *portalItem = qobject_cast<CPortalItem*>(algBItem);
        if ((portalItem != nullptr) && (portalItem->portal() != nullptr))
            emit portalChecked(portalItem->portal(), (portalItem->checkState() == Qt::Checked));

        break;
    }
    }
}

void CAlgTreeModel::setSchemes(const QList<CScheme*> &schemes)
{
    clear();

    foreach (CScheme *scheme, schemes) {
        addScheme(scheme);
    }
}

void CAlgTreeModel::addScheme(CScheme *scheme)
{
    if (scheme == nullptr)
        return;
    if (isContains(scheme))
        return;

    CAlgBuffModelItem *schemeItem = new CSchemeItem(scheme, this);
    connect(schemeItem, SIGNAL(itemChanged(QStandardItem*,int)),
            this, SLOT(onItemChanged(QStandardItem*,int)));
    appendRow(schemeItem);
}