#include "cdatawindow.h"

#include <QTabWidget>
#include <QTableView>

#include <qwt/qwt_plot_grid.h>
#include <qwt/qwt_legend.h>
#include <qwt/qwt_plot_curve.h>

#include "../scheme/cscheme.h"
#include "../scheme/algorithm/calgorithm.h"
#include "../scheme/portal/cargument.h"
#include "../scheme/portal/cresult.h"
#include "../scheme/databuffer/cabstractdatabuffer.h"

/*!
 * \class CDataTable
 */
int CDataTable::maxBuffCount(void) const
{
	int maxCount = 0;
	foreach(CAbstractDataBuffer *buf, m_buffers)
	{
		if(!buf) continue;
		if(buf->count() > maxCount) maxCount = buf->count();
	}
	return maxCount;
}

CDataTable::CDataTable(QObject *parent) : QAbstractTableModel(parent)
{
	setObjectName(QStringLiteral("CDataTable"));

	m_maxBuffCount = 0;
	m_skipUpdatesInterval = 10;
	m_skipUpdatesCounter = 0;
}

void CDataTable::setBuffers(const QList<CAbstractDataBuffer*> &buffers)
{
	foreach(CAbstractDataBuffer *buf, m_buffers)
	{
		disconnect(buf, SIGNAL(dataAdded(int,qreal)), this, SLOT(updateModel()));
		disconnect(buf, SIGNAL(destroyed(QObject*)), this, SLOT(onBufferDestroyed(QObject*)));
	}
	m_buffers = buffers;
	foreach(CAbstractDataBuffer *buf, m_buffers)
	{
		connect(buf, SIGNAL(dataAdded(int,qreal)), this, SLOT(updateModel()));
		connect(buf, SIGNAL(destroyed(QObject*)), this, SLOT(onBufferDestroyed(QObject*)));
	}
	updateModel();
}

void CDataTable::addBuffer(CAbstractDataBuffer *buffer)
{
	if(!buffer) return;
	if(m_buffers.contains(buffer)) return;

	m_buffers << buffer;
	connect(buffer, SIGNAL(dataAdded(int,qreal)), this, SLOT(onBufferDataAdded(int,qreal)));
	connect(buffer, SIGNAL(destroyed(QObject*)), this, SLOT(onBufferDestroyed(QObject*)));

	updateModel();
}

int CDataTable::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return m_maxBuffCount;
}

int CDataTable::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return m_buffers.count();
}

QVariant CDataTable::data(const QModelIndex &index, int role) const
{
	if(!index.isValid()) return QVariant();
	if(index.column() >= m_buffers.count()) return QVariant();

	CAbstractDataBuffer *buf = m_buffers.at(index.column());
	if(!buf) return QVariant();
	if(index.row() >= buf->count()) return QVariant();

	switch(role)
	{
		case Qt::DisplayRole: return QVariant(buf->data(index.row()));
	}

	return QVariant();
}

void CDataTable::updateModel(void)
{
	m_maxBuffCount = maxBuffCount();
	emit layoutAboutToBeChanged();
	emit layoutChanged();
}

void CDataTable::onBufferDataAdded(const int &timeFrame, const qreal &data)
{
	Q_UNUSED(timeFrame)
	Q_UNUSED(data)

	if(m_skipUpdatesCounter >= m_skipUpdatesInterval)
	{
		updateModel();
		m_skipUpdatesCounter = 0;
	}
	else
	{
		++m_skipUpdatesCounter;
	}
}

void CDataTable::onBufferDestroyed(QObject *objBuffer)
{
	if(m_buffers.contains((CAbstractDataBuffer*)objBuffer)) m_buffers.removeOne((CAbstractDataBuffer*)objBuffer);
	updateModel();
}

void CDataTable::flushBuffers(void)
{
	updateModel();
	m_skipUpdatesCounter = 0;
}

/*!
 * \class CCurveData
 */
CCurveData::CCurveData(CAbstractDataBuffer *buffer)
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
	if(!m_buffer) return QwtArraySeriesData::sample/*<QPointF>*/(index);
	return QPointF(index, m_buffer->data(index));
}

void CCurveData::setBuffer(CAbstractDataBuffer *buffer)
{
	if(m_buffer && (m_buffer == buffer)) return;
	m_buffer = buffer;
}

/*!
 * \class CCurve
 */
void CCurve::initConstructor(CAbstractDataBuffer *buffer)
{
	m_buffer = 0;
	m_curveData = 0;

	m_curveData = new CCurveData(0);
	setData(m_curveData);

	setBuffer(buffer);
}

CCurve::CCurve(CAbstractDataBuffer *buffer, const QString &title) : QwtPlotCurve(title)
{
	initConstructor(buffer);
}

CCurve::CCurve(CAbstractDataBuffer *buffer, const QwtText &title) : QwtPlotCurve(title)
{
	initConstructor(buffer);
}

void CCurve::setBuffer(CAbstractDataBuffer *buffer)
{
	if(m_buffer && (m_buffer == buffer)) return;
	if(m_buffer)
	{
		if(m_curveData) m_curveData->setBuffer(0);
	}
	m_buffer = buffer;
	if(m_buffer)
	{
		if(m_curveData) m_curveData->setBuffer(m_buffer);
	}
}

/*!
 * \class CDataPlot
 */
CDataPlot::CDataPlot(QWidget *parent) : QwtPlot(parent)
{
	setObjectName(QStringLiteral("CPlot"));

    m_colorIndex = 0;
	m_skipUpdatesInterval = 0;
	m_skipUpdatesCounter = 0;
	m_grid = 0;

	m_grid = new QwtPlotGrid();
	m_grid->attach(this);

    setCanvasBackground(QColor(88, 88, 88, 128));
	insertLegend(new QwtLegend());
}

void CDataPlot::setBuffers(const QList<CAbstractDataBuffer*> &dataBuffers)
{
	for(int ci = 0; ci < m_bufferCurveMap.count(); ++ci)
	{
		if(m_bufferCurveMap.keys().at(ci))
		{
			disconnect(m_bufferCurveMap.keys().at(ci), SIGNAL(dataAdded(int,qreal)), this, SLOT(onBufferDataAdded(int,qreal)));
			disconnect(m_bufferCurveMap.keys().at(ci), SIGNAL(destroyed(QObject*)), this, SLOT(onBufferDestroyed(QObject*)));
		}
		if(m_bufferCurveMap.values().at(ci))
		{
			m_bufferCurveMap.values().at(ci)->detach();
			delete m_bufferCurveMap.values().at(ci);
		}
	}
    m_colorIndex = 0;
    m_bufferCurveMap.clear();

    QStringList colors = QColor::colorNames();
	foreach(CAbstractDataBuffer *buffer, dataBuffers)
	{
		if(!buffer) continue;
		if(m_bufferCurveMap.contains(buffer)) continue;

		connect(buffer, SIGNAL(dataAdded(int,qreal)), this, SLOT(onBufferDataAdded(int,qreal)));
		connect(buffer, SIGNAL(destroyed(QObject*)), this, SLOT(onBufferDestroyed(QObject*)));

        QColor color = colors.at(m_colorIndex);
        ++m_colorIndex;
        if(m_colorIndex >= colors.count()) m_colorIndex = 0;

		CCurve *curve = new CCurve(buffer);
        curve->setPen(color, 2.0);
		curve->attach(this);
		m_bufferCurveMap[buffer] = curve;
	}

	replot();
    updateLayout();
}

void CDataPlot::addBuffer(CAbstractDataBuffer *buffer)
{
	if(!buffer) return;
	if(m_bufferCurveMap.contains(buffer)) return;

	connect(buffer, SIGNAL(dataAdded(int,qreal)), this, SLOT(onBufferDataAdded(int,qreal)));
	connect(buffer, SIGNAL(destroyed(QObject*)), this, SLOT(onBufferDestroyed(QObject*)));

    QStringList colors = QColor::colorNames();
    ++m_colorIndex;
    if(m_colorIndex >= colors.count()) m_colorIndex = 0;
    QColor color = colors.at(m_colorIndex);

    CCurve *curve = new CCurve(buffer);
    curve->setPen(color, 2);
	curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
	curve->attach(this);
	m_bufferCurveMap[buffer] = curve;

	replot();
    updateLayout();
}

void CDataPlot::onBufferDataAdded(const int &timeFrame, const qreal &data)
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

void CDataPlot::onBufferDestroyed(QObject *objBuffer)
{
	if(m_bufferCurveMap.contains((CAbstractDataBuffer*)objBuffer))
	{
		if(m_bufferCurveMap[(CAbstractDataBuffer*)objBuffer])
		{
			m_bufferCurveMap[(CAbstractDataBuffer*)objBuffer]->detach();
			delete m_bufferCurveMap[(CAbstractDataBuffer*)objBuffer];
		}
		m_bufferCurveMap.remove((CAbstractDataBuffer*)objBuffer);
	}
	replot();
}

void CDataPlot::flushBuffers(void)
{
	replot();
	m_skipUpdatesCounter = 0;
}

/*!
 * \class CDataWindow
 */
void CDataWindow::addPortals(const QList<CPortal*> &portals)
{
	QList<CAbstractDataBuffer*> argBuffers;
	QList<CAbstractDataBuffer*> resBuffers;
	foreach(CPortal *portal, portals)
	{
        if(portal->buffer())
        {
            if(qobject_cast<CArgument*>(portal))
            {
                if(!argBuffers.contains(portal->buffer())) argBuffers << portal->buffer();
            }
            else if(qobject_cast<CResult*>(portal))
            {
                if(!resBuffers.contains(portal->buffer())) resBuffers << portal->buffer();
            }
        }
    }
	if(m_argDataPlot) m_argDataPlot->setBuffers(argBuffers);
	if(m_argDataTable) m_argDataTable->setBuffers(argBuffers);
	if(m_resDataPlot) m_resDataPlot->setBuffers(resBuffers);
	if(m_resDataTable) m_resDataTable->setBuffers(resBuffers);
}

void CDataWindow::addPortals(const QList<CAlgorithm*> &algorithms)
{
	QList<CPortal*> portals;
	foreach(CAlgorithm *alg, algorithms)
	{
		if(alg) portals << alg->portals();
	}
	addPortals(portals);
}

void CDataWindow::addPortal(CPortal *portal)
{
	if(!portal) return;

    if(portal->buffer())
    {
        if(qobject_cast<CArgument*>(portal))
        {
            if(m_argDataPlot) m_argDataPlot->addBuffer(portal->buffer());
            if(m_argDataTable) m_argDataTable->addBuffer(portal->buffer());
        }
        else if(qobject_cast<CResult*>(portal))
        {
            if(m_resDataPlot) m_resDataPlot->addBuffer(portal->buffer());
            if(m_resDataTable) m_resDataTable->addBuffer(portal->buffer());
        }
    }
}

CDataWindow::CDataWindow(QWidget *parent) : QMainWindow(parent)
{
	setObjectName(QStringLiteral("CDataWindow"));
	setWindowFlags(Qt::Dialog);

	m_argResTabs = 0;
	m_argTabs = 0;
	m_resTabs = 0;
    m_argDataPlot = 0;
    m_argDataTable = 0;
    m_resDataPlot = 0;
    m_resDataTable = 0;

	m_argResTabs = new QTabWidget();
	m_argResTabs->setObjectName(QStringLiteral("argResTabs"));
	setCentralWidget(m_argResTabs);

	m_argTabs = new QTabWidget();
	m_argTabs->setObjectName(QStringLiteral("argTabs"));
	m_argTabs->setWindowTitle(tr("Arguments"));
	m_argResTabs->addTab(m_argTabs, m_argTabs->windowTitle());

	m_resTabs = new QTabWidget();
	m_resTabs->setObjectName("resTabs");
	m_resTabs->setWindowTitle(tr("Results"));
	m_argResTabs->addTab(m_resTabs, m_resTabs->windowTitle());

    m_argDataPlot = new CDataPlot();
    m_argDataPlot->setObjectName(QStringLiteral("argDataPlot"));
    m_argDataPlot->setWindowTitle(tr("Arguments plot"));
    m_argTabs->addTab(m_argDataPlot, m_argDataPlot->windowTitle());

    m_resDataPlot = new CDataPlot();
    m_resDataPlot->setObjectName(QStringLiteral("resDataPlot"));
    m_resDataPlot->setWindowTitle(tr("Results plot"));
    m_resTabs->addTab(m_resDataPlot, m_resDataPlot->windowTitle());

    QTableView *argTabView = new QTableView();
    argTabView->setObjectName(QStringLiteral("argTabView"));
    argTabView->setWindowTitle("Arguments table");
    m_argTabs->addTab(argTabView, argTabView->windowTitle());

    QTableView *resTabView = new QTableView();
    resTabView->setObjectName(QStringLiteral("resTabView"));
    resTabView->setWindowTitle("Results table");
    m_resTabs->addTab(resTabView, resTabView->windowTitle());

    m_argDataTable = new CDataTable(this);
    m_argDataTable->setObjectName(QLatin1String("argDataTable"));
    argTabView->setModel(m_argDataTable);

    m_resDataTable = new CDataTable(this);
    m_resDataTable->setObjectName(QLatin1String("resDataTable"));
    resTabView->setModel(m_resDataTable);
}

void CDataWindow::onAlgorithmDestroyed(QObject *objAlgorithm)
{
	if(!objAlgorithm) return;
	if(m_algorithms.contains((CAlgorithm*)objAlgorithm)) m_algorithms.removeOne((CAlgorithm*)objAlgorithm);
}

void CDataWindow::onPortalAdded(CPortal *portal)
{
	if(!portal) return;
	addPortal(portal);
}

void CDataWindow::setAlgorithms(const QList<CAlgorithm*> &algorithms)
{
	foreach(CAlgorithm *alg, m_algorithms)
	{
		if(!qobject_cast<CAlgorithm*>(alg)) continue;

		disconnect(alg, SIGNAL(portalAdded(CPortal*)), this, SLOT(onPortalAdded(CPortal*)));
		disconnect(alg, SIGNAL(destroyed(QObject*)), this, SLOT(onAlgorithmDestroyed(QObject*)));
	}
	m_algorithms = algorithms;
	foreach(CAlgorithm *alg, m_algorithms)
	{
		if(!qobject_cast<CAlgorithm*>(alg)) continue;

		connect(alg, SIGNAL(portalAdded(CPortal*)), this, SLOT(onPortalAdded(CPortal*)));
		connect(alg, SIGNAL(destroyed(QObject*)), this, SLOT(onAlgorithmDestroyed(QObject*)));
	}
	if(isVisible()) addPortals(m_algorithms);
}

void CDataWindow::flushBuffers(void)
{
	if(m_argDataPlot) m_argDataPlot->flushBuffers();
	if(m_argDataTable) m_argDataTable->flushBuffers();
	if(m_resDataPlot) m_resDataPlot->flushBuffers();
	if(m_resDataTable) m_resDataTable->flushBuffers();
}

void CDataWindow::setVisible(bool visible)
{
	QMainWindow::setVisible(visible);
	if(isVisible())
	{
		addPortals(m_algorithms);
	}
	else
	{
		addPortals(QList<CAlgorithm*>());
	}
}

//void CDataWindow::setAlgorithms(const QList<CAlgorithm*> &algorithms)
//{
//	foreach(QwtPlotCurve *curve, m_curves)
//	{
//		if(curve)
//		{
//			curve->detach();
//			delete curve;
//		}
//	}
//	m_curves.clear();

//	foreach(CAlgorithm *alg, algorithms)
//	{
//		if(!alg) continue;

//		QList<CResult*> results = alg->results();
//		foreach(CResult *res, results)
//		{
//			if(!res) continue;
//			if(res->buffer() && !res->buffer()->isEmpty())
//			{
//				QwtPlotCurve *curve = new QwtPlotCurve();
//				curve->setTitle(res->name());
//				curve->setPen(Qt::blue, 1);
//				curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);

//				QVector<QPointF> points;
//				for(int ci = 0; ci < res->buffer()->count(); ++ci)
//				{
//					points << QPointF(ci, res->buffer()->data(ci));
//				}
//				curve->setSamples(points);

//				m_curves << curve;
//				curve->attach(this);
//			}
//		}
//	}
//	replot();
//}