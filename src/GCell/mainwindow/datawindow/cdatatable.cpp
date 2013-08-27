#include "cdatatable.h"

#include <qmath.h>

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
	m_skipUpdatesInterval = 1000;
	m_skipUpdatesCounter = 0;
}

void CDataTable::addPortal(CPortal *portal)
{
	if(!portal) return;
	if(!portal->buffer()) return;
	if(m_portals.contains(portal)) return;

	m_portals << portal;
	connect(portal->buffer(), SIGNAL(dataAppended(stTimeFrame,stData)), this, SLOT(onBufferDataAppended(stTimeFrame,stData)));
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
			disconnect(portal->buffer(), SIGNAL(dataAppended(stTimeFrame,stData)), this, SLOT(onBufferDataAppended(stTimeFrame,stData)));
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
	return m_portals.count()*2;
}

QVariant CDataTable::headerData(int section, Qt::Orientation orientation, int role) const
{
	switch(role)
	{
		case Qt::DisplayRole:
		{
			if(orientation == Qt::Horizontal)
			{				
				int sec = qFloor((qreal)section/2.0);
				if(sec < 0 || m_portals.count() <= sec) break;
				if(!(section%2))//even
				{
					if(!m_portals.at(sec)) break;
					return QVariant(m_portals.at(sec)->caption() + tr(":Time"));
				}
				else//odd
				{
					if(!m_portals.at(sec)) break;
					return QVariant(m_portals.at(sec)->caption() + tr(":Values"));
				}
			}
		}
	}

	return QAbstractTableModel::headerData(section, orientation, role);
}

QVariant CDataTable::data(const QModelIndex &index, int role) const
{
	if(!index.isValid()) return QVariant();

	int col = qFloor((qreal)index.column()/2.0);
	if(col < 0 || col >= m_portals.count()) return QVariant();

	CDataBuffer *buf = m_portals.at(col)->buffer();
	if(!buf) return QVariant();
	if(index.row() >= buf->count()) return QVariant();

	switch(role)
	{
		case Qt::DisplayRole:
		{
			if(!(index.column()&1))//even
			{
				return QVariant(buf->data(index.row()).timeFrame.time);
			}
			else//odd
			{
				return QVariant(buf->data(index.row()).value);
			}
		}
		case Qt::TextColorRole: return QVariant(m_portals.at(col)->dataColor());
	}

	return QVariant();
}

void CDataTable::onBufferDataAppended(const stTimeFrame &timeFrame, const stData &data)
{
	Q_UNUSED(timeFrame)
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