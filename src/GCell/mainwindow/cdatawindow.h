#ifndef CDATAWINDOW_H
#define CDATAWINDOW_H

#include <qwt/qwt_plot.h>

class QwtPlotGrid;
class QwtPlotCurve;

class CAlgorithm;

class CDataWindow : public QwtPlot
{
	Q_OBJECT
private:
	QwtPlotGrid *m_grid;
	QList<QwtPlotCurve*> m_curves;
public:
	explicit CDataWindow(QWidget *parent = 0);
public slots:
	void setAlgorithms(const QList<CAlgorithm*> &algorithms);
};

#endif // CDATAWINDOW_H