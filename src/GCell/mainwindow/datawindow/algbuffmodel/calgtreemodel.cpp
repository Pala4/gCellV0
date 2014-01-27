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
        if (algBItem->portal() && (algBItem->checkState() == Qt::Checked))
            checkedPortals << algBItem->portal();
    } else {
        for (int childIndex = 0; childIndex < algBItem->rowCount(); ++childIndex)
            checkedPortals << getCheckedPortalsRecurrence(algBItem->child(childIndex));
    }

    return checkedPortals;
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
        CPortal *portal = algBItem->portal();
        if (portal == nullptr)
            return;

        emit portalChecked(portal, (algBItem->checkState() == Qt::Checked));
        break;
    }
    }
}

void CAlgTreeModel::setSchemes(const QList<CScheme*> &schemes)
{
    clear();

    foreach (CScheme *scheme, schemes) {
        if (scheme == nullptr)
            continue;
        if (scheme->algorithms().isEmpty())
            continue;

        CAlgBuffModelItem *schemeItem = new CAlgBuffModelItem(scheme);
        connect(schemeItem, SIGNAL(itemChanged(QStandardItem*,int)),
                this, SLOT(onItemChanged(QStandardItem*,int)));
        appendRow(schemeItem);
    }
}
