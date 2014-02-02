#include "cdatatable.h"

#include <qmath.h>

#include "../../scheme/portal/cportal.h"
#include "algbuffmodel/calgtreemodel.h"

/*!
 * \class CDataTable
 */
unsigned long long CDataTable::maxBuffSize() const
{
    unsigned long long maxCount = 0ULL;
    foreach (CPortal *portal, m_portals) {
        if (portal == nullptr)
            continue;
        if (portal->bufferSize() > maxCount)
            maxCount = portal->bufferSize();
    }

    return maxCount;
}

CDataTable::CDataTable(QObject *parent) : QAbstractTableModel(parent)
{
    setObjectName(QStringLiteral("CDataTable"));

    m_maxBuffSize = 0;
    m_algTreeModel = 0;
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

    if (index.row() >= (int)m_portals.at(col)->bufferSize())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    {
        if (!(index.column()&1)) {//even
            return QVariant((qreal)m_portals.at(col)->bufferData(index.row()).ldblTimeFrame);
        } else {//odd
            return QVariant((qreal)m_portals.at(col)->bufferData(index.row()).ldblValue);
        }
    }
    case Qt::TextColorRole:
        return QVariant(m_portals.at(col)->dataColor());
    default:
        break;
    }

    return QVariant();
}

void CDataTable::setAlgTreeModel(CAlgTreeModel *algTreeModel)
{
    if(m_algTreeModel && (m_algTreeModel == algTreeModel)) return;
    if(m_algTreeModel)
    {
        disconnect(m_algTreeModel, SIGNAL(destroyed()), this, SLOT(onAlgTreeModelDestroyed()));
        disconnect(m_algTreeModel, SIGNAL(layoutChanged()), this, SLOT(rebuild()));
        disconnect(m_algTreeModel, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(rebuild()));
        disconnect(m_algTreeModel, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(rebuild()));
        disconnect(m_algTreeModel, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(rebuild()));
        disconnect(m_algTreeModel, SIGNAL(columnsInserted(QModelIndex,int,int)), this, SLOT(rebuild()));
        disconnect(m_algTreeModel, SIGNAL(columnsRemoved(QModelIndex,int,int)), this, SLOT(rebuild()));
        disconnect(m_algTreeModel, SIGNAL(columnsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(rebuild()));
        disconnect(m_algTreeModel, SIGNAL(modelReset()), this, SLOT(rebuild()));
        disconnect(m_algTreeModel, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(rebuild()));
        disconnect(m_algTreeModel, SIGNAL(headerDataChanged(Qt::Orientation,int,int)), this, SLOT(rebuild()));
    }
    m_algTreeModel = algTreeModel;
    if(m_algTreeModel)
    {
        connect(m_algTreeModel, SIGNAL(destroyed()), this, SLOT(onAlgTreeModelDestroyed()));
        connect(m_algTreeModel, SIGNAL(layoutChanged()), this, SLOT(rebuild()));
        connect(m_algTreeModel, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(rebuild()));
        connect(m_algTreeModel, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(rebuild()));
        connect(m_algTreeModel, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(rebuild()));
        connect(m_algTreeModel, SIGNAL(columnsInserted(QModelIndex,int,int)), this, SLOT(rebuild()));
        connect(m_algTreeModel, SIGNAL(columnsRemoved(QModelIndex,int,int)), this, SLOT(rebuild()));
        connect(m_algTreeModel, SIGNAL(columnsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(rebuild()));
        connect(m_algTreeModel, SIGNAL(modelReset()), this, SLOT(rebuild()));
        connect(m_algTreeModel, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(rebuild()));
        connect(m_algTreeModel, SIGNAL(headerDataChanged(Qt::Orientation,int,int)), this, SLOT(rebuild()));
    }
    rebuild();
}

void CDataTable::onAlgTreeModelDestroyed(void)
{
    m_algTreeModel = 0;
    rebuild();
}

void CDataTable::rebuild(void)
{
    m_portals = QList<CPortal*>();
    if(m_algTreeModel) m_portals = m_algTreeModel->checkedPortalList();

    refresh();
}

void CDataTable::refresh(void)
{
    emit layoutAboutToBeChanged();
    m_maxBuffSize = maxBuffSize();
    emit layoutChanged();
}