#include "cdataplot.h"

#include <qwt/qwt_plot_grid.h>
#include <qwt/qwt_legend.h>
#include <qwt/qwt_plot_curve.h>

#include "../../scheme/portal/cportal.h"

/*!
 * \class CCurveData
 */
CCurveData::CCurveData(CDataBuffer *buffer)
{
	m_buffer = 0;

	setBuffer(buffer);
}

QRectF CCurveData::boundingRect(void) const
{
	d_boundingRect = qwtBoundingRect(*this);
	return d_boundingRect;
}

size_t CCurveData::size(void) const
{
	if(!m_buffer) return QwtArraySeriesData::size();
	return m_buffer->count();
}

QPointF CCurveData::sample(size_t index) const
{
	if(!m_buffer) return QwtArraySeriesData::sample(index);
	return QPointF(m_buffer->data(index).timeFrame.time, m_buffer->data(index).value);
}

void CCurveData::setBuffer(CDataBuffer *buffer)
{
	if(m_buffer && (m_buffer == buffer)) return;
	m_buffer = buffer;
}

/*!
 * \class CCurve
 */
void CCurve::initConstructor(CPortal *portal)
{
	m_portal = 0;
	m_curveData = 0;

	m_curveData = new CCurveData(0);
	setData(m_curveData);

	setPortal(portal);
}

CCurve::CCurve(CPortal *portal, const QString &title) : QwtPlotCurve(title)
{
	initConstructor(portal);
}

CCurve::CCurve(CPortal *portal, const QwtText &title) : QwtPlotCurve(title)
{
	initConstructor(portal);
}

void CCurve::setPortal(CPortal *portal)
{
	if(m_portal && (m_portal == portal)) return;
	if(m_portal)
	{
		if(m_curveData) m_curveData->setBuffer(0);
	}
	m_portal = portal;
	if(m_portal && m_portal->buffer())
	{
		if(m_curveData) m_curveData->setBuffer(m_portal->buffer());
	}
}

/*!
 * \class CDataPlot
 */
CDataPlot::CDataPlot(QWidget *parent) : QwtPlot(parent)
{
	setObjectName(QStringLiteral("CPlot"));

	m_skipUpdatesInterval = 1000;
	m_skipUpdatesCounter = 0;
	m_grid = 0;

	m_grid = new QwtPlotGrid();
	m_grid->attach(this);

	setCanvasBackground(QColor(150, 150, 150));
	insertLegend(new QwtLegend());
}

void CDataPlot::addPortal(CPortal *portal)
{
	if(!portal) return;
	if(!portal->buffer()) return;
	if(m_portalCurveMap.contains(portal)) return;

	connect(portal->buffer(), SIGNAL(dataAppended(stTimeFrame,stData)), this, SLOT(onBufferDataAppended(stTimeFrame,stData)));
	connect(portal->buffer(), SIGNAL(cleared()), this, SLOT(onBufferCleared()));
	connect(portal, SIGNAL(destroyed(QObject*)), this, SLOT(onPortalDestroyed(QObject*)));

	CCurve *curve = new CCurve(portal, portal->caption());
	curve->setPen(portal->dataColor(), 3);
	curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
	curve->attach(this);
	m_portalCurveMap[portal] = curve;
}

void CDataPlot::addPortals(const QList<CPortal*> &portals)
{
	foreach(CPortal *portal, portals)
	{
		addPortal(portal);
	}

	replot();
	updateLayout();
}

void CDataPlot::clearPortals(void)
{
	for(int ci = 0; ci < m_portalCurveMap.count(); ++ci)
	{
		if(m_portalCurveMap.keys().at(ci))
		{
			if(m_portalCurveMap.keys().at(ci)->buffer())
			{
				disconnect(m_portalCurveMap.keys().at(ci)->buffer(), SIGNAL(dataAppended(stTimeFrame,stData)), this, SLOT(onBufferDataAppended(stTimeFrame,stData)));
				disconnect(m_portalCurveMap.keys().at(ci)->buffer(), SIGNAL(cleared()), this, SLOT(onBufferCleared()));
			}
			disconnect(m_portalCurveMap.keys().at(ci), SIGNAL(destroyed(QObject*)), this, SLOT(onPortalDestroyed(QObject*)));
		}
		if(m_portalCurveMap.values().at(ci))
		{
			m_portalCurveMap.values().at(ci)->detach();
			delete m_portalCurveMap.values().at(ci);
		}
	}
	m_portalCurveMap.clear();
}

void CDataPlot::onBufferDataAppended(const stTimeFrame &timeFrame, const stData &data)
{
	Q_UNUSED(timeFrame)
	Q_UNUSED(data)

	if(m_skipUpdatesCounter >= m_skipUpdatesInterval)
	{
		replot();
		m_skipUpdatesCounter = 0;
	}
	else
	{
		++m_skipUpdatesCounter;
	}
}

void CDataPlot::onBufferCleared(void)
{
	replot();
}

void CDataPlot::onPortalDestroyed(QObject *objPortal)
{
	if(!objPortal) return;
	CPortal *portal = (CPortal*)objPortal;
	if(m_portalCurveMap.contains(portal))
	{
		if(m_portalCurveMap[portal])
		{
			m_portalCurveMap[portal]->detach();
			delete m_portalCurveMap[portal];
		}
		m_portalCurveMap.remove(portal);
	}
	refresh();
}

void CDataPlot::flush(void)
{
	m_skipUpdatesCounter = 0;
	refresh();
}

void CDataPlot::refresh(void)
{
	replot();
	updateLayout();
}