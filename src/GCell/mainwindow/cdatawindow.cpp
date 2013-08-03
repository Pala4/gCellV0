#include "cdatawindow.h"

#include <qwt/qwt_plot_grid.h>
#include <qwt/qwt_legend.h>
#include <qwt/qwt_plot_curve.h>

#include "../scheme/algorithm/calgorithm.h"
#include "../scheme/portal/cresult.h"
#include "../scheme/databuffer/cabstractdatabuffer.h"

CDataWindow::CDataWindow(QWidget *parent) : QwtPlot(parent)
{
	setObjectName(QStringLiteral("CDataWindow"));
	setWindowFlags(Qt::Dialog);

	m_grid = 0;

	m_grid = new QwtPlotGrid();
	m_grid->attach(this);

	setCanvasBackground(Qt::white);
	insertLegend(new QwtLegend());
}

void CDataWindow::setAlgorithms(const QList<CAlgorithm*> &algorithms)
{
	foreach(QwtPlotCurve *curve, m_curves)
	{
		if(curve)
		{
			curve->detach();
			delete curve;
		}
	}
	m_curves.clear();

	foreach(CAlgorithm *alg, algorithms)
	{
		if(!alg) continue;

		QList<CResult*> results = alg->results();
		foreach(CResult *res, results)
		{
			if(!res) continue;
			if(res->buffer() && !res->buffer()->isEmpty())
			{
				QwtPlotCurve *curve = new QwtPlotCurve();
				curve->setTitle(res->name());
				curve->setPen(Qt::blue, 4),
				curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);

				QVector<QPointF> points;
				for(int ci = 0; ci < res->buffer()->count(); ++ci)
				{
					points << QPointF(ci, res->buffer()->data(ci));
				}
				curve->setSamples(points);

				m_curves << curve;
				curve->attach(this);
			}
		}
	}
	replot();
}