#ifndef CDATAWINDOW_H
#define CDATAWINDOW_H

#include <QMainWindow>

class QSplitter;
class QTabWidget;
class QToolButton;
class QSpinBox;

class CAlgorithm;
class CDataPlot;
class CDataTable;
class CAlgTreeModel;

class CDataWindow : public QMainWindow
{
	Q_OBJECT
	Q_PROPERTY(bool autoRefresh READ isAutoRefresh WRITE setAutoRefresh)
	Q_PROPERTY(int autoRefreshInterval READ autoRefreshInterval WRITE setAutoRefreshInterval)
private:
	QList<CAlgorithm*> m_algorithms;
	QSplitter *m_splMain;
	QTabWidget *m_tabWgt;
	CDataPlot *m_dataPlot;
	CDataTable *m_dataTable;
	CAlgTreeModel *m_algTreeModel;
	bool m_autoRefreshStopped;
	int m_refreshTimerID;
	QToolButton *m_autoRefreshToolBt;
	QSpinBox *m_autoRefreshIntervalSpBx;
private:
	void stopAutoRefreshProcedure(void);
protected:
	virtual void timerEvent(QTimerEvent *event);
public:
	explicit CDataWindow(QWidget *parent = 0);

	bool isAutoRefresh(void);
	void setAutoRefresh(const bool &autoRefresh);
	int autoRefreshInterval(void);
	void setAutoRefreshInterval(const int &autoRefreshInterval);

	QSplitter* splitter(void){return m_splMain;}
public slots:
	void setAlgorithms(const QList<CAlgorithm*> &algorithms);
	void startAutoRefresh(void);
	void stopAutoRefresh(void);
	void refresh(void);
};

#endif // CDATAWINDOW_H