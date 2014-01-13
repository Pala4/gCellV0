#include "cdatawindow.h"

#include <QCloseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTabWidget>
#include <QTableView>
#include <QTreeView>
#include <QTimerEvent>
#include <QToolButton>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>

#include "cdataplot.h"
#include "cdatatable.h"
#include "algbuffmodel/calgtreemodel.h"

/*!
 * \class CDataWindow
 */
void CDataWindow::timerEvent(QTimerEvent *event)
{
	if(event->timerId() == m_refreshTimerID)
	{
		refresh();

		if(m_autoRefreshStopped)
		{
			killTimer(m_refreshTimerID);
			m_refreshTimerID = 0;
			if(m_autoRefreshToolBt) m_autoRefreshToolBt->setEnabled(true);
			if(m_autoRefreshIntervalSpBx) m_autoRefreshIntervalSpBx->setEnabled(true);
		}
	}
}

CDataWindow::CDataWindow(QWidget *parent) : QMainWindow(parent)
{
	setObjectName(QStringLiteral("CDataWindow"));
	setWindowFlags(Qt::Dialog);

	m_splMain = 0;
	m_tabWgt = 0;
	m_dataPlot = 0;
	m_dataTable = 0;
	m_algTreeModel = 0;
	m_autoRefreshStopped = true;
	m_refreshTimerID = 0;
	m_autoRefreshToolBt = 0;
	m_autoRefreshIntervalSpBx = 0;

	QWidget *wgtMain = new QWidget();
	setCentralWidget(wgtMain);

	QVBoxLayout *vbl = new QVBoxLayout();
	wgtMain->setLayout(vbl);

	m_splMain = new QSplitter(Qt::Horizontal);
	vbl->addWidget(m_splMain);

	QTreeView *algTreeView = new QTreeView();
	m_splMain->addWidget(algTreeView);

	m_tabWgt = new QTabWidget();
	m_tabWgt->setObjectName(QStringLiteral("tabWgt"));
	m_splMain->addWidget(m_tabWgt);

	m_splMain->setStretchFactor(0, 1);
	m_splMain->setStretchFactor(1, 2);

	m_dataPlot = new CDataPlot();
	m_dataPlot->setObjectName(QStringLiteral("dataPlot"));
	m_dataPlot->setWindowTitle(tr("Plot"));
	m_tabWgt->addTab(m_dataPlot, m_dataPlot->windowTitle());

	QTableView *tabView = new QTableView();
	tabView->setObjectName(QStringLiteral("tabView"));
	tabView->setWindowTitle("Table");
	m_tabWgt->addTab(tabView, tabView->windowTitle());

	m_dataTable = new CDataTable(this);
	m_dataTable->setObjectName(QLatin1String("dataTable"));
	tabView->setModel(m_dataTable);

	m_algTreeModel = new CAlgTreeModel(this);
	m_algTreeModel->setObjectName(QStringLiteral("algBuffModel"));
	algTreeView->setModel(m_algTreeModel);
	m_dataPlot->setAlgTreeModel(m_algTreeModel);
	m_dataTable->setAlgTreeModel(m_algTreeModel);

	QHBoxLayout *hblRefreshCtrls = new QHBoxLayout();
	vbl->addLayout(hblRefreshCtrls);

	m_autoRefreshToolBt = new QToolButton();
	m_autoRefreshToolBt->setObjectName(QStringLiteral("autoRefreshToolBt"));
	m_autoRefreshToolBt->setText(tr("Auto refresh"));
	m_autoRefreshToolBt->setCheckable(true);
	m_autoRefreshToolBt->setChecked(true);
	hblRefreshCtrls->addWidget(m_autoRefreshToolBt);

	m_autoRefreshIntervalSpBx = new QSpinBox();
	m_autoRefreshIntervalSpBx->setObjectName(QStringLiteral("autoRefreshIntervalSpBx"));
	m_autoRefreshIntervalSpBx->setRange(1, 10*60*1000/*10 min*/);
	m_autoRefreshIntervalSpBx->setValue(5);
	connect(m_autoRefreshToolBt, SIGNAL(clicked(bool)), m_autoRefreshIntervalSpBx, SLOT(setEnabled(bool)));
	connect(m_autoRefreshToolBt, SIGNAL(toggled(bool)), m_autoRefreshIntervalSpBx, SLOT(setEnabled(bool)));
	hblRefreshCtrls->addWidget(new QLabel(tr("every")));
	hblRefreshCtrls->addWidget(m_autoRefreshIntervalSpBx);
	hblRefreshCtrls->addWidget(new QLabel(tr("ms")));

	QPushButton *pbRefresh = new QPushButton(tr("Refresh"));
	pbRefresh->setDisabled(m_autoRefreshToolBt->isChecked());
	connect(pbRefresh, SIGNAL(clicked()), this, SLOT(refresh()));
	connect(m_autoRefreshToolBt, SIGNAL(clicked(bool)), pbRefresh, SLOT(setDisabled(bool)));
	connect(m_autoRefreshToolBt, SIGNAL(toggled(bool)), pbRefresh, SLOT(setDisabled(bool)));
	hblRefreshCtrls->addWidget(pbRefresh);
	hblRefreshCtrls->addStretch();
}

bool CDataWindow::isAutoRefresh(void)
{
	return m_autoRefreshToolBt ? m_autoRefreshToolBt->isChecked() : false;
}

void CDataWindow::setAutoRefresh(const bool &autoRefresh)
{
	if(m_autoRefreshToolBt) m_autoRefreshToolBt->setChecked(autoRefresh);
}

int CDataWindow::autoRefreshInterval(void)
{
	return m_autoRefreshIntervalSpBx ? m_autoRefreshIntervalSpBx->value() : 5;
}

void CDataWindow::setAutoRefreshInterval(const int &autoRefreshInterval)
{
	if(m_autoRefreshIntervalSpBx) m_autoRefreshIntervalSpBx->setValue(autoRefreshInterval);
}

void CDataWindow::setVisible(bool visible)
{
	QMainWindow::setVisible(visible);
	emit visibleChanged(visible);
}

void CDataWindow::setAlgorithms(const QList<CAlgorithm*> &algorithms)
{
	if(m_algTreeModel) m_algTreeModel->setAlgorithms(algorithms);
}

void CDataWindow::startAutoRefresh(void)
{
	if(!m_autoRefreshToolBt || !m_autoRefreshToolBt->isChecked()) return;
	if(!m_autoRefreshStopped) return;

	int refreshInterval = 5;
	if(m_autoRefreshIntervalSpBx)
	{
		refreshInterval = m_autoRefreshIntervalSpBx->value();
		m_autoRefreshIntervalSpBx->setEnabled(false);
	}
	m_autoRefreshToolBt->setEnabled(false);

	m_refreshTimerID = startTimer(refreshInterval);
	m_autoRefreshStopped = false;
}

void CDataWindow::stopAutoRefresh(void)
{
	if(!m_autoRefreshToolBt || !m_autoRefreshToolBt->isChecked()) return;
	if(m_autoRefreshStopped) return;
	if(m_refreshTimerID == 0) return;

	m_autoRefreshStopped = true;
}

void CDataWindow::refresh(void)
{
	if(m_dataPlot) m_dataPlot->refresh();
	if(m_dataTable) m_dataTable->refresh();
}