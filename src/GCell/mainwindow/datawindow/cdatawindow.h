#ifndef CDATAWINDOW_H
#define CDATAWINDOW_H

#include <QMainWindow>

#include "../../scheme/databuffer/cdatabuffer.h"

class QTabWidget;

class CAlgorithm;
class CPortal;
class CDataPlot;
class CDataTable;
class CAlgBuffModel;

class CDataWindow : public QMainWindow
{
	Q_OBJECT
private:
	QList<CAlgorithm*> m_algorithms;
	QTabWidget *m_tabWgt;
	CDataPlot *m_dataPlot;
	CDataTable *m_dataTable;
	CAlgBuffModel *m_algBuffModel;

	void addPortal(CPortal *portal);
	void addArguments(const QList<CPortal*> &arguments);
	void addResults(const QList<CPortal*> &results);
	void addPortals(const QList<CAlgorithm*> &algorithms);
public:
	explicit CDataWindow(QWidget *parent = 0);	
private slots:
	void onAlgorithmDestroyed(QObject *objAlgorithm);
	void onPortalAdded(CPortal *portal);
	void updateViews(void);
public slots:
	void setAlgorithms(const QList<CAlgorithm*> &algorithms);
	void flushBuffers(void);

	virtual void setVisible(bool visible);
};

#endif // CDATAWINDOW_H