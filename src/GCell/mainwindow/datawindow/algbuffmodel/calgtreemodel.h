#ifndef CALGTREEMODEL_H
#define CALGTREEMODEL_H

#include <QStandardItemModel>

class CScheme;
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
    void setSchemes(const QList<CScheme*> &schemes);
signals:
	void portalChecked(CPortal *portal, bool checked);
};

#endif // CALGTREEMODEL_H
