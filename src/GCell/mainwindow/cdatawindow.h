#ifndef CDATAWINDOW_H
#define CDATAWINDOW_H

#include <QMainWindow>
#include <QAbstractTableModel>

#include <qwt/qwt_plot.h>
#include <qwt/qwt_series_data.h>
#include <qwt/qwt_plot_curve.h>

class QTabWidget;

class QwtPlotGrid;
class QwtPlotCurve;

class CScheme;
class CAlgorithm;
class CPortal;
class CAbstractDataBuffer;

class CDataTable : public QAbstractTableModel
{
	Q_OBJECT
private:
	int m_maxBuffCount;
	int m_skipUpdatesInterval;
	int m_skipUpdatesCounter;
	QList<CAbstractDataBuffer*> m_buffers;

	int maxBuffCount(void) const;
public:
	explicit CDataTable(QObject *parent = 0);

	void setBuffers(const QList<CAbstractDataBuffer*> &buffers);
	void addBuffer(CAbstractDataBuffer *buffer);

	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
private slots:
	void updateModel(void);
	void onBufferDataAdded(const int &timeFrame, const qreal &data);
	void onBufferDestroyed(QObject *objBuffer);
public slots:
	void flushBuffers(void);
};

class CCurveData : public QwtArraySeriesData<QPointF>
{
private:
	CAbstractDataBuffer *m_buffer;
public:
	CCurveData(CAbstractDataBuffer *buffer);

	virtual QRectF boundingRect(void) const;
	virtual size_t size(void) const;
	virtual QPointF sample(size_t index) const;

	void setBuffer(CAbstractDataBuffer *buffer);
};

class CCurve : public QwtPlotCurve
{
private:
	CAbstractDataBuffer *m_buffer;
	CCurveData *m_curveData;

	void initConstructor(CAbstractDataBuffer *buffer);
public:
	explicit CCurve(CAbstractDataBuffer *buffer, const QString &title = QString::null);
	explicit CCurve(CAbstractDataBuffer *buffer, const QwtText &title);

	void setBuffer(CAbstractDataBuffer *buffer);
};

class CDataPlot : public QwtPlot
{
	Q_OBJECT
private:
    int m_colorIndex;
	int m_skipUpdatesInterval;
	int m_skipUpdatesCounter;
	QMap<CAbstractDataBuffer*, QwtPlotCurve*> m_bufferCurveMap;
	QwtPlotGrid *m_grid;
public:
	explicit CDataPlot(QWidget *parent = 0);

	void setBuffers(const QList<CAbstractDataBuffer*> &dataBuffers);
	void addBuffer(CAbstractDataBuffer *buffer);

    virtual QSize minimumSizeHint(void) const{return QSize(0, 0);}
private slots:
	void onBufferDataAdded(const int &timeFrame, const qreal &data);
	void onBufferDestroyed(QObject *objBuffer);
public slots:
	void flushBuffers(void);
};

class CDataWindow : public QMainWindow
{
	Q_OBJECT
private:
	QList<CAlgorithm*> m_algorithms;
	QTabWidget *m_argResTabs;
	QTabWidget *m_argTabs;
	QTabWidget *m_resTabs;
    CDataPlot *m_argDataPlot;
    CDataTable *m_argDataTable;
    CDataPlot *m_resDataPlot;
    CDataTable *m_resDataTable;

	void addPortals(const QList<CPortal*> &portals);
	void addPortals(const QList<CAlgorithm*> &algorithms);
	void addPortal(CPortal *portal);
public:
	explicit CDataWindow(QWidget *parent = 0);
private slots:
	void onAlgorithmDestroyed(QObject *objAlgorithm);
	void onPortalAdded(CPortal *portal);
public slots:
	void setAlgorithms(const QList<CAlgorithm*> &algorithms);
	void flushBuffers(void);

	virtual void setVisible(bool visible);
};

#endif // CDATAWINDOW_H