#ifndef CDATATABLE_H
#define CDATATABLE_H

#include <QAbstractTableModel>

#include "../../scheme/databuffer/cdatabuffer.h"

class CPortal;
class CAlgTreeModel;

class CDataTable : public QAbstractTableModel
{
	Q_OBJECT
private:
	int m_maxBuffSize;
	CAlgTreeModel *m_algTreeModel;
	QList<CPortal*> m_portals;

    unsigned long long maxBuffSize() const;
public:
	explicit CDataTable(QObject *parent = 0);

	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	CAlgTreeModel* algTreeModel(void){return m_algTreeModel;}
	void setAlgTreeModel(CAlgTreeModel *algTreeModel);
private slots:
	void onAlgTreeModelDestroyed(void);
public slots:
	void rebuild(void);
	void refresh(void);
};

#endif // CDATATABLE_H