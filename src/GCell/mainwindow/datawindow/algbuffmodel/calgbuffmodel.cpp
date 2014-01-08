#include "calgbuffmodel.h"

#include "calgbuffmodelitem.h"
#include "../../../scheme/algorithm/calgorithm.h"

CAlgBuffModel::CAlgBuffModel(QObject *parent) : QStandardItemModel(parent)
{
	setObjectName(QStringLiteral("CAlgBuffModel"));
}

void CAlgBuffModel::setAlgorithms(const QList<CAlgorithm *> &algs)
{
	clear();

	foreach(CAlgorithm *alg, algs)
	{
		if(!alg) continue;
		if(alg->arguments().isEmpty() && alg->results().isEmpty()) continue;

		CAlgBuffModelItem *algItem = new CAlgBuffModelItem(alg);
		appendRow(algItem);
		if(!alg->arguments().isEmpty())
		{
			CAlgBuffModelItem *argNode = new CAlgBuffModelItem(tr("Arguments"));
			algItem->appendRow(argNode);
			foreach(CPortal *arg, alg->argPortals())
			{
				if(!arg) continue;
				CAlgBuffModelItem *argItem = new CAlgBuffModelItem(arg);
				argNode->appendRow(argItem);
			}
		}
		if(!alg->results().isEmpty())
		{
			CAlgBuffModelItem *resNode = new CAlgBuffModelItem(tr("Results"));
			algItem->appendRow(resNode);
			foreach(CPortal *res, alg->resPortals())
			{
				if(!res) continue;
				CAlgBuffModelItem *resItem = new CAlgBuffModelItem(res);
				resNode->appendRow(resItem);
			}
		}
	}
}