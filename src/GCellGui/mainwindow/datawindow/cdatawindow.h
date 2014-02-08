#ifndef CDATAWINDOW_H
#define CDATAWINDOW_H

#include <QMainWindow>

class QSplitter;
class QTabWidget;
class QCheckBox;
class QSpinBox;
class QPushButton;
class QTimer;

class CAlgorithm;
class CScheme;
class CDataPlot;
class CDataTable;
class CAlgTreeModel;

class CDataWindow : public QMainWindow
{
	Q_OBJECT
    Q_PROPERTY(bool autoRefresh READ isAutoRefreshEnabled WRITE setAutoRefresh)
	Q_PROPERTY(int autoRefreshInterval READ autoRefreshInterval WRITE setAutoRefreshInterval)
private:
	QList<CAlgorithm*> m_algorithms;
	QSplitter *m_splMain;
	QTabWidget *m_tabWgt;
	CDataPlot *m_dataPlot;
	CDataTable *m_dataTable;
	CAlgTreeModel *m_algTreeModel;
    QTimer *m_autoRefreshTimer;
    bool m_autoRefreshStarted;
    QCheckBox *m_autoRefreshChBx;
	QSpinBox *m_autoRefreshIntervalSpBx;
    QPushButton *m_pbRefresh;
public:
	explicit CDataWindow(QWidget *parent = 0);

    bool isAutoRefreshEnabled(void);
	void setAutoRefresh(const bool &autoRefresh);
	int autoRefreshInterval(void);
	void setAutoRefreshInterval(const int &autoRefreshInterval);

	QSplitter* splitter(void){return m_splMain;}
private slots:
    void onAutoRefreshChBxStateChanged(int state);
public slots:
    void setVisible(bool visible);

    void setSchemes(const QList<CScheme*> &schemes);
    void addScheme(CScheme *scheme);
	void startAutoRefresh(void);
	void stopAutoRefresh(void);
	void refresh(void);
signals:
	void visibleChanged(bool visible);
};

#endif // CDATAWINDOW_H
