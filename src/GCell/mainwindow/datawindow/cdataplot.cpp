#include "cdataplot.h"

#include <qwt/qwt_plot_grid.h>
#include <qwt/qwt_legend.h>
#include <qwt/qwt_plot_curve.h>

#include "../../scheme/portal/cportal.h"
#include "algbuffmodel/calgtreemodel.h"

/*!
 * \class CCurveData
 */
CCurveData::CCurveData(CPortal *portal)
{
	m_portal = 0;

	setPortal(portal);
}

QRectF CCurveData::boundingRect(void) const
{
	d_boundingRect = qwtBoundingRect(*this);
	return d_boundingRect;
}

size_t CCurveData::size(void) const
{
	if(!m_portal) return QwtArraySeriesData::size();
	return m_portal->bufferSize();
}

QPointF CCurveData::sample(size_t index) const
{
	if(!m_portal) return QwtArraySeriesData::sample(index);
	return QPointF(m_portal->bufferData(index).timeFrame.time, m_portal->bufferData(index).value);
}

void CCurveData::setPortal(CPortal *portal)
{
	if(m_portal && (m_portal == portal)) return;
	m_portal = portal;
}

/*!
 * \class CDataPlot
 */
CDataPlot::CDataPlot(QWidget *parent) : QwtPlot(parent)
{
	setObjectName(QStringLiteral("CPlot"));

	m_grid = 0;
	m_algTreeModel = 0;

	m_grid = new QwtPlotGrid();
	m_grid->attach(this);

	setCanvasBackground(QColor(255, 255, 255));
	insertLegend(new QwtLegend());
}

void CDataPlot::setAlgTreeModel(CAlgTreeModel *algTreeModel)
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

void CDataPlot::onAlgTreeModelDestroyed(void)
{
	m_algTreeModel = 0;
	rebuild();
}

void CDataPlot::rebuild(void)
{
	for(int ci = 0; ci < m_portalCurveMap.count(); ++ci)
	{
		if(m_portalCurveMap.values().at(ci))
		{
			m_portalCurveMap.values().at(ci)->detach();
			delete m_portalCurveMap.values().at(ci);
		}
	}
	m_portalCurveMap.clear();

	if(m_algTreeModel)
	{
		QList<CPortal*> portals = m_algTreeModel->checkedPortalList();
		foreach(CPortal *portal, portals)
		{
			if(!portal) continue;
			QwtPlotCurve *curve = new QwtPlotCurve(portal->caption());
			curve->setData(new CCurveData(portal));
			curve->setPen(portal->dataColor(), 3);
			curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
			curve->attach(this);
			m_portalCurveMap[portal] = curve;
		}
	}

	refresh();
}

void CDataPlot::refresh(void)
{
	replot();
	updateLayout();
}