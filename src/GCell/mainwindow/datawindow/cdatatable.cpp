#include "cdatatable.h"

#include "../../scheme/algorithm/calgorithm.h"
#include "../../scheme/portal/cportal.h"
#include "../../scheme/portal/cargument.h"
#include "../../scheme/portal/cresult.h"

/*!
 * \class CDataTable
 */
int CDataTable::maxBuffSize(void) const
{
	int maxCount = 0;
	foreach(CPortal *portal, m_portals)
	{
		if(!portal) continue;
		if(!portal->buffer()) continue;
		if(portal->buffer()->count() > maxCount) maxCount = portal->buffer()->count();
	}
	return maxCount;
}

CDataTable::CDataTable(QObject *parent) : QAbstractTableModel(parent)
{
	setObjectName(QStringLiteral("CDataTable"));

	m_maxBuffSize = 0;
	m_skipUpdatesInterval = 10;
	m_skipUpdatesCounter = 0;
}

void CDataTable::addPortal(CPortal *portal)
{
	if(!portal) return;
	if(!portal->buffer()) return;
	if(m_portals.contains(portal)) return;

	m_portals << portal;
	connect(portal->buffer(), SIGNAL(dataAppended(stData)), this, SLOT(onBufferDataAppended(stData)));
	connect(portal->buffer(), SIGNAL(cleared()), this, SLOT(refresh()));
	connect(portal, SIGNAL(destroyed(QObject*)), this, SLOT(onPortalDestroyed(QObject*)));
}

void CDataTable::addPortals(const QList<CPortal*> &portals)
{
	foreach(CPortal *portal, portals)
	{
		addPortal(portal);
	}
	refresh();
}

void CDataTable::clearPortals(void)
{
	foreach(CPortal *portal, m_portals)
	{
		if(!portal) continue;
		if(portal->buffer())
		{
			disconnect(portal->buffer(), SIGNAL(dataAppended(stData)), this, SLOT(onBufferDataAppended(stData)));
			disconnect(portal->buffer(), SIGNAL(cleared()), this, SLOT(refresh()));
		}
		disconnect(portal, SIGNAL(destroyed(QObject*)), this, SLOT(onPortalDestroyed(QObject*)));
	}
	m_portals.clear();
	refresh();
}

int CDataTable::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return m_maxBuffSize;
}

int CDataTable::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return m_portals.count();
}

QVariant CDataTable::headerData(int section, Qt::Orientation orientation, int role) const
{
	switch(role)
	{
		case Qt::DisplayRole:
		{
			if(orientation == Qt::Horizontal)
			{
				if(section < 0 || m_portals.count() <= section) break;
				if(!m_portals.at(section)) break;
				return QVariant(m_portals.at(section)->caption());
			}
		}
	}

	return QAbstractTableModel::headerData(section, orientation, role);
}

QVariant CDataTable::data(const QModelIndex &index, int role) const
{
	if(!index.isValid()) return QVariant();
	if(index.column() < 0 || index.column() >= m_portals.count()) return QVariant();

	CDataBuffer *buf = m_portals.at(index.column())->buffer();
	if(!buf) return QVariant();
	if(index.row() >= buf->count()) return QVariant();

	switch(role)
	{
		case Qt::DisplayRole: return QVariant(buf->data(index.row()).value);
		case Qt::TextColorRole: return QVariant(m_portals.at(index.column())->dataColor());
	}

	return QVariant();
}

void CDataTable::onBufferDataAppended(const stData &data)
{
	Q_UNUSED(data)

	if(m_skipUpdatesCounter >= m_skipUpdatesInterval)
	{
		refresh();
		m_skipUpdatesCounter = 0;
	}
	else
	{
		++m_skipUpdatesCounter;
	}
}

void CDataTable::onPortalDestroyed(QObject *objPortal)
{
	if(!objPortal) return;
	CPortal *portal = (CPortal*)objPortal;
	if(m_portals.contains(portal)) m_portals.removeOne(portal);
	refresh();
}

void CDataTable::refresh(void)
{
	m_maxBuffSize = maxBuffSize();
	emit layoutAboutToBeChanged();
	emit layoutChanged();
}

void CDataTable::flush(void)
{
	refresh();
	m_skipUpdatesCounter = 0;
}