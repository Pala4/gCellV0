#include "cdatawindow.h"

#include <QCloseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTabWidget>
#include <QTableView>
#include <QTreeView>
#include <QTimerEvent>
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QTimer>

#include "cdataplot.h"
#include "cdatatable.h"
#include "algbuffmodel/calgtreemodel.h"

/*!
 * \class CDataWindow
 */
CDataWindow::CDataWindow(QWidget *parent) : QMainWindow(parent)
{
	setObjectName(QStringLiteral("CDataWindow"));
	setWindowFlags(Qt::Dialog);
    setWindowTitle(tr("Data window"));

    m_splMain = nullptr;
    m_tabWgt = nullptr;
    m_dataPlot = nullptr;
    m_dataTable = nullptr;
    m_algTreeModel = nullptr;
    m_autoRefreshTimer = nullptr;
    m_autoRefreshStarted = false;
    m_autoRefreshChBx = nullptr;
    m_autoRefreshIntervalSpBx = nullptr;
    m_pbRefresh = nullptr;

	QWidget *wgtMain = new QWidget();
	setCentralWidget(wgtMain);

	QVBoxLayout *vbl = new QVBoxLayout();
	wgtMain->setLayout(vbl);

	m_splMain = new QSplitter(Qt::Horizontal);
	vbl->addWidget(m_splMain);

	QTreeView *algTreeView = new QTreeView();
    algTreeView->setHeaderHidden(true);
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

    m_autoRefreshChBx = new QCheckBox(tr("Auto refresh"), this);
    m_autoRefreshChBx->setObjectName(QStringLiteral("autoRefreshChBx"));
    m_autoRefreshChBx->setCheckState(Qt::Unchecked);
    connect(m_autoRefreshChBx, SIGNAL(stateChanged(int)),
            this, SLOT(onAutoRefreshChBxStateChanged(int)));
    hblRefreshCtrls->addWidget(m_autoRefreshChBx);

	m_autoRefreshIntervalSpBx = new QSpinBox();
	m_autoRefreshIntervalSpBx->setObjectName(QStringLiteral("autoRefreshIntervalSpBx"));
    m_autoRefreshIntervalSpBx->setEnabled(isAutoRefreshEnabled());
	m_autoRefreshIntervalSpBx->setRange(1, 10*60*1000/*10 min*/);
	m_autoRefreshIntervalSpBx->setValue(5);
	hblRefreshCtrls->addWidget(new QLabel(tr("every")));
	hblRefreshCtrls->addWidget(m_autoRefreshIntervalSpBx);
	hblRefreshCtrls->addWidget(new QLabel(tr("ms")));

    m_pbRefresh = new QPushButton(tr("Refresh"), this);
    m_pbRefresh->setObjectName(QStringLiteral("pbRefresh"));
    m_pbRefresh->setDisabled(isAutoRefreshEnabled());
    connect(m_pbRefresh, SIGNAL(clicked()), this, SLOT(refresh()));
    hblRefreshCtrls->addWidget(m_pbRefresh);
	hblRefreshCtrls->addStretch();
}

bool CDataWindow::isAutoRefreshEnabled(void)
{
    return (m_autoRefreshChBx != nullptr) ? (m_autoRefreshChBx->checkState() == Qt::Checked)
                                          : false;
}

void CDataWindow::setAutoRefresh(const bool &autoRefresh)
{
    if (m_autoRefreshChBx != nullptr)
        m_autoRefreshChBx->setCheckState(autoRefresh ? Qt::Checked : Qt::Unchecked);
}

int CDataWindow::autoRefreshInterval(void)
{
    return (m_autoRefreshIntervalSpBx != nullptr) ? m_autoRefreshIntervalSpBx->value() : 5;
}

void CDataWindow::setAutoRefreshInterval(const int &autoRefreshInterval)
{
    if (m_autoRefreshIntervalSpBx != nullptr)
        m_autoRefreshIntervalSpBx->setValue(autoRefreshInterval);
}

void CDataWindow::onAutoRefreshChBxStateChanged(int state)
{
    bool autoRefreshEnabled = (state == Qt::Checked);

    if (m_autoRefreshIntervalSpBx != nullptr)
        m_autoRefreshIntervalSpBx->setEnabled(autoRefreshEnabled);
    if (m_pbRefresh != nullptr)
        m_pbRefresh->setDisabled(autoRefreshEnabled);

    if (m_autoRefreshTimer != nullptr) {
        if (!autoRefreshEnabled) {
            if (m_autoRefreshTimer->isActive())
                m_autoRefreshTimer->stop();
        } else {
            if (!m_autoRefreshTimer->isActive())
                m_autoRefreshTimer->start(autoRefreshInterval());
        }
    }
    else
        refresh();
}

void CDataWindow::setVisible(bool visible)
{
	QMainWindow::setVisible(visible);
	emit visibleChanged(visible);
}

void CDataWindow::setSchemes(const QList<CScheme*> &schemes)
{
    if (m_algTreeModel)
        m_algTreeModel->setSchemes(schemes);
}

void CDataWindow::addScheme(CScheme *scheme)
{
    if (m_algTreeModel)
        m_algTreeModel->addScheme(scheme);
}

void CDataWindow::startAutoRefresh(void)
{
    if (!isAutoRefreshEnabled())
        return;

    if (m_autoRefreshTimer == nullptr) {
        m_autoRefreshTimer = new QTimer(this);
        m_autoRefreshTimer->setObjectName(QStringLiteral("autoRefreshTimer"));
        connect(m_autoRefreshTimer, SIGNAL(timeout()), this, SLOT(refresh()));
    }
    if ((m_autoRefreshTimer != nullptr) && !m_autoRefreshTimer->isActive())
        m_autoRefreshTimer->start(autoRefreshInterval());
}

void CDataWindow::stopAutoRefresh(void)
{
    if (m_autoRefreshTimer != nullptr) {
        if (m_autoRefreshTimer->isActive())
            m_autoRefreshTimer->stop();

        m_autoRefreshTimer->deleteLater();
        m_autoRefreshTimer = nullptr;
    }
}

void CDataWindow::refresh(void)
{
    if (m_dataPlot != nullptr)
        m_dataPlot->refresh();
    if (m_dataTable != nullptr)
        m_dataTable->refresh();
}
