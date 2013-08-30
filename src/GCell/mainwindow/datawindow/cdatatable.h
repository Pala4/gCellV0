#ifndef CDATATABLE_H
#define CDATATABLE_H

#include <QAbstractTableModel>

#include "../../scheme/databuffer/cdatabuffer.h"

class CPortal;

class CDataTable : public QAbstractTableModel
{
	Q_OBJECT
private:
	int m_maxBuffSize;
	int m_skipUpdatesInterval;
	int m_skipUpdatesCounter;
	QList<CPortal*> m_portals;

	quint64 maxBuffSize(void) const;
public:
	explicit CDataTable(QObject *parent = 0);

	void addPortal(CPortal *portal);
	void addPortals(const QList<CPortal*> &portals);
	void clearPortals(void);

	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
private slots:
	void onBufferDataAppended(const stTimeFrame &timeFrame, const stData &data);
	void onPortalDestroyed(QObject *objPortal);
public slots:
	void refresh(void);
	void flush(void);
};

#endif // CDATATABLE_H