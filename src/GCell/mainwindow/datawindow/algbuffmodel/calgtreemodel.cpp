#include "calgtreemodel.h"

#include "calgbuffmodelitem.h"
#include "../../../scheme/algorithm/calgorithm.h"

QList<CPortal*> CAlgTreeModel::getCheckedPortalsRecurrence(QStandardItem *item)
{
	QList<CPortal*> checkedPortals;
	CAlgBuffModelItem *algBItem = dynamic_cast<CAlgBuffModelItem*>(item);
	if(!algBItem) return checkedPortals;

	if(algBItem->type() == CAlgBuffModelItem::Portal)
	{
		if(algBItem->portal() &&
		   algBItem->checkState() == Qt::Checked)
		{
			checkedPortals << algBItem->portal();
		}
	}
	else
	{
		for(int childIndex = 0; childIndex < algBItem->rowCount(); ++childIndex)
		{
			checkedPortals << getCheckedPortalsRecurrence(algBItem->child(childIndex));
		}
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
	for(int rowIndex = 0; rowIndex < rowCount(); ++rowIndex)
	{
		checkedPortals << getCheckedPortalsRecurrence(item(rowIndex));
	}
	return checkedPortals;
}

void CAlgTreeModel::onItemChanged(QStandardItem *item, const int &role)
{
	CAlgBuffModelItem *algBItem = dynamic_cast<CAlgBuffModelItem*>(item);
	if(!algBItem) return;

	switch(role)
	{
		case Qt::CheckStateRole:
		{
			CPortal *portal = algBItem->portal();
			if(!portal) return;

			emit portalChecked(portal, (algBItem->checkState() == Qt::Checked));
		}
		break;
	}
}

void CAlgTreeModel::setAlgorithms(const QList<CAlgorithm*> &algs)
{
	clear();

	foreach(CAlgorithm *alg, algs)
	{
		if(!alg) continue;
		if(alg->arguments().isEmpty() && alg->results().isEmpty()) continue;

		CAlgBuffModelItem *algItem = new CAlgBuffModelItem(alg);
		connect(algItem, SIGNAL(itemChanged(QStandardItem*,int)), this, SLOT(onItemChanged(QStandardItem*,int)));
		appendRow(algItem);
	}
}