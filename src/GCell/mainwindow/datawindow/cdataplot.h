#ifndef CDATAPLOT_H
#define CDATAPLOT_H

#include <qwt/qwt_plot.h>
#include <qwt/qwt_series_data.h>
#include <qwt/qwt_plot_curve.h>

#include "../../scheme/databuffer/cdatabuffer.h"

class QwtPlotGrid;
class QwtPlotCurve;

class CPortal;

class CCurveData : public QwtArraySeriesData<QPointF>
{
private:
	CDataBuffer *m_buffer;
public:
	CCurveData(CDataBuffer *buffer);

	virtual QRectF boundingRect(void) const;
	virtual size_t size(void) const;
	virtual QPointF sample(size_t index) const;

	void setBuffer(CDataBuffer *buffer);
};

class CCurve : public QwtPlotCurve
{
private:
	CPortal *m_portal;
	CCurveData *m_curveData;

	void initConstructor(CPortal *portal);
public:
	explicit CCurve(CPortal *portal, const QString &title = QString::null);
	explicit CCurve(CPortal *portal, const QwtText &title);

	void setPortal(CPortal *portal);
};

class CDataPlot : public QwtPlot
{
	Q_OBJECT
private:
	int m_skipUpdatesInterval;
	int m_skipUpdatesCounter;
	QMap<CPortal*, QwtPlotCurve*> m_portalCurveMap;
	QwtPlotGrid *m_grid;
public:
	explicit CDataPlot(QWidget *parent = 0);

	void addPortal(CPortal *portal);
	void addPortals(const QList<CPortal*> &portals);
	void clearPortals(void);

//	virtual QSize minimumSizeHint(void) const{return QSize(0, 0);}
private slots:
	void onBufferDataAppended(const stData &data);
	void onBufferCleared(void);
	void onPortalDestroyed(QObject *objPortal);
public slots:
	void flush(void);
	void refresh(void);
};

#endif // CDATAPLOT_H