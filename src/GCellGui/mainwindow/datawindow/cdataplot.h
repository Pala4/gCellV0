#ifndef CDATAPLOT_H
#define CDATAPLOT_H

#include <qwt/qwt_plot.h>
#include <qwt/qwt_series_data.h>
#include <qwt/qwt_plot_curve.h>

class QwtPlotGrid;
class QwtPlotCurve;

class CPortal;
class CAlgTreeModel;

class CCurveData : public QwtArraySeriesData<QPointF>
{
private:
	CPortal *m_portal;
public:
	CCurveData(CPortal *portal);

	virtual QRectF boundingRect(void) const;
	virtual size_t size(void) const;
	virtual QPointF sample(size_t index) const;

	void setPortal(CPortal *portal);
};

class CDataPlot : public QwtPlot
{
	Q_OBJECT
private:
	CAlgTreeModel *m_algTreeModel;
	QMap<CPortal*, QwtPlotCurve*> m_portalCurveMap;
	QwtPlotGrid *m_grid;
public:
	explicit CDataPlot(QWidget *parent = 0);

	virtual QSize minimumSizeHint(void) const{return QSize(0, 0);}

	CAlgTreeModel* algTreeModel(void){return m_algTreeModel;}
	void setAlgTreeModel(CAlgTreeModel *algTreeModel);
private slots:
	void onAlgTreeModelDestroyed(void);
public slots:
	void rebuild(void);
	void refresh(void);
};

#endif // CDATAPLOT_H