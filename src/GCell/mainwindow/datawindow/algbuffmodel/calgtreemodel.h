#ifndef CALGTREEMODEL_H
#define CALGTREEMODEL_H

#include <QStandardItemModel>

class CAlgorithm;
class CPortal;

class CAlgTreeModel : public QStandardItemModel
{
	Q_OBJECT
private:
	QList<CPortal*> getCheckedPortalsRecurrence(QStandardItem *item);
public:
	explicit CAlgTreeModel(QObject *parent = 0);

	QList<CPortal*> checkedPortalList(void);
private slots:
	void onItemChanged(QStandardItem *item, const int &role);
public slots:
	void setAlgorithms(const QList<CAlgorithm*> &algs);
signals:
	void portalChecked(CPortal *portal, bool checked);
};

#endif // CALGTREEMODEL_H