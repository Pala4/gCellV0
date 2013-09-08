#include "coptionswindow.h"

#include <QVBoxLayout>
#include <QTabWidget>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QMessageBox>

/*!
 * \class COptionsWindow
 */
COptionsWindow::COptionsWindow(QWidget *parent) : QDialog(parent)
{
	setObjectName(QStringLiteral("COptionsWindow"));

	m_autoSaveConfigChBox = 0;
	m_autoSaveDesktopChBox = 0;
	m_autoSaveLastSchemeChBox = 0;
	m_autoLoadLastSchemeChBox = 0;

	m_gridColorLEdit = 0;
	m_gridBkGndColorLEdit = 0;
	m_gridStepSpBox = 0;
	m_gridPointSize = 0;
	m_gridAlignChBox = 0;

	m_schemeTab = 0;
	m_schemeWidthSpBox = 0;
	m_schemeHeightSpBox = 0;

	m_calcParamTab = 0;
	m_startTimeSpBox = 0;
	m_timeStepSpBox = 0;
	m_endTimeSpBox = 0;

	QVBoxLayout *vbLayout = new QVBoxLayout();
	setLayout(vbLayout);

	QTabWidget *optGroupTabWgt = new QTabWidget();
	optGroupTabWgt->setObjectName(QStringLiteral("optGroupTabWgt"));
	vbLayout->addWidget(optGroupTabWgt);

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
													   Qt::Horizontal);
	vbLayout->addWidget(buttonBox);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	//General tab
	QWidget *generalTab = new QWidget();
	generalTab->setObjectName(QStringLiteral("generalTab"));
	generalTab->setWindowTitle(tr("General"));
	optGroupTabWgt->addTab(generalTab, generalTab->windowTitle());

	QVBoxLayout *generalLayout = new QVBoxLayout();
	generalTab->setLayout(generalLayout);

	m_autoSaveConfigChBox = new QCheckBox(tr("Auto save config"));
	m_autoSaveConfigChBox->setObjectName(QStringLiteral("autoSaveConfigChBx"));
	m_autoSaveConfigChBox->setCheckable(true);
	generalLayout->addWidget(m_autoSaveConfigChBox);

	m_autoSaveDesktopChBox = new QCheckBox(tr("Auto save desktop"));
	m_autoSaveDesktopChBox->setObjectName(QStringLiteral("autoSaveDesktopChBx"));
	m_autoSaveDesktopChBox->setCheckable(true);
	generalLayout->addWidget(m_autoSaveDesktopChBox);

	m_autoSaveLastSchemeChBox = new QCheckBox(tr("Auto save last scheme"));
	m_autoSaveLastSchemeChBox->setObjectName(QStringLiteral("autoSaveLastSchemeChBx"));
	m_autoSaveLastSchemeChBox->setCheckable(true);
	generalLayout->addWidget(m_autoSaveLastSchemeChBox);

	m_autoLoadLastSchemeChBox = new QCheckBox(tr("Auto load last scheme"));
	m_autoLoadLastSchemeChBox->setObjectName(QStringLiteral("autoLoadLastSchemeChBx"));
	m_autoLoadLastSchemeChBox->setCheckable(true);
	generalLayout->addWidget(m_autoLoadLastSchemeChBox);

	//Scheme editor tab
	QWidget *schemeEditorTab = new QWidget();
	schemeEditorTab->setObjectName(QStringLiteral("schemeEditorTab"));
	schemeEditorTab->setWindowTitle(tr("Scheme editor"));
	optGroupTabWgt->addTab(schemeEditorTab, schemeEditorTab->windowTitle());

	QFormLayout *flSchemeEditor = new QFormLayout();
	schemeEditorTab->setLayout(flSchemeEditor);

	m_gridColorLEdit = new CColorLineEdit();
	m_gridColorLEdit->setObjectName(QStringLiteral("gridColorLEdit"));
	flSchemeEditor->addRow(tr("Grid color:"), m_gridColorLEdit);

	m_gridBkGndColorLEdit = new CColorLineEdit();
	m_gridBkGndColorLEdit->setObjectName(QStringLiteral("gridBkGndColorLEdit"));
	flSchemeEditor->addRow(tr("Grid background color:"), m_gridBkGndColorLEdit);

	m_gridStepSpBox = new QSpinBox();
	m_gridStepSpBox->setObjectName(QStringLiteral("gridStepSpBox"));
	flSchemeEditor->addRow(tr("Grid step:"), m_gridStepSpBox);

	m_gridPointSize = new QDoubleSpinBox();
	m_gridPointSize->setObjectName(QStringLiteral("gridPointSize"));
	m_gridPointSize->setRange(0.0, 3.0);
	flSchemeEditor->addRow(tr("Grid point size:"), m_gridPointSize);

	m_gridAlignChBox = new QCheckBox(tr("Align by grid"));
	m_gridAlignChBox->setObjectName(QStringLiteral("gridAlignChBox"));
	m_gridAlignChBox->setCheckable(true);
	flSchemeEditor->addRow(m_gridAlignChBox);

	//Scheme tab
	m_schemeTab = new QWidget();
	m_schemeTab->setObjectName(QStringLiteral("schemeTab"));
	m_schemeTab->setWindowTitle(tr("Active scheme"));
	optGroupTabWgt->addTab(m_schemeTab, m_schemeTab->windowTitle());

	QFormLayout *flScheme = new QFormLayout();
	m_schemeTab->setLayout(flScheme);

	QHBoxLayout *hbSchemeSize = new QHBoxLayout();
	m_schemeWidthSpBox = new QDoubleSpinBox();
	m_schemeWidthSpBox->setObjectName(QStringLiteral("schemeWidthSpBox"));
	m_schemeWidthSpBox->setRange(0.0, 30000);
	hbSchemeSize->addWidget(m_schemeWidthSpBox, 1);
	QLabel *xLabel = new QLabel(QStringLiteral("x"));
	hbSchemeSize->addWidget(xLabel, 0);
	m_schemeHeightSpBox = new QDoubleSpinBox();
	m_schemeHeightSpBox->setObjectName(QStringLiteral("schemeHeightSpBox"));
	m_schemeHeightSpBox->setRange(0.0, 30000);
	hbSchemeSize->addWidget(m_schemeHeightSpBox, 1);

	flScheme->addRow(tr("Scheme size (W x H):"), hbSchemeSize);

	//Calc param tab
	m_calcParamTab = new QWidget();
	m_calcParamTab->setObjectName(QStringLiteral("calcParamWgt"));
	m_calcParamTab->setWindowTitle(tr("Calculation parameters"));
	optGroupTabWgt->addTab(m_calcParamTab, m_calcParamTab->windowTitle());

	QFormLayout *flTimeLine = new QFormLayout();
	m_calcParamTab->setLayout(flTimeLine);

	m_startTimeSpBox = new QDoubleSpinBox();
	m_startTimeSpBox->setDecimals(3);
	m_startTimeSpBox->setMaximum(999999.0);
	flTimeLine->addRow(tr("Start time:"), m_startTimeSpBox);

	m_timeStepSpBox = new QDoubleSpinBox();
	m_timeStepSpBox->setDecimals(3);
	m_timeStepSpBox->setValue(1.0);
	m_timeStepSpBox->setMaximum(999999.0);
	flTimeLine->addRow(tr("Time step:"), m_timeStepSpBox);

	m_endTimeSpBox = new QDoubleSpinBox();
	m_endTimeSpBox->setDecimals(3);
	m_endTimeSpBox->setMaximum(999999.0);
	flTimeLine->addRow(tr("End time:"), m_endTimeSpBox);

	setWindowTitle(tr("Options"));
}

bool COptionsWindow::isAutoSaveConfig(void) const
{
	return (m_autoSaveConfigChBox) ? (m_autoSaveConfigChBox->checkState() == Qt::Checked) : false;
}

void COptionsWindow::setAutoSaveConfig(const bool &autoSaveConfig)
{
	if(m_autoSaveConfigChBox) m_autoSaveConfigChBox->setChecked(autoSaveConfig);
}

bool COptionsWindow::isAutoSaveDesktop(void) const
{
	return (m_autoSaveDesktopChBox) ? (m_autoSaveDesktopChBox->checkState() == Qt::Checked) : false;
}

void COptionsWindow::setAutoSaveDesktop(const bool &autoSaveDesktop)
{
	if(m_autoSaveDesktopChBox) m_autoSaveDesktopChBox->setChecked(autoSaveDesktop);
}

bool COptionsWindow::isAutoSaveLastScheme(void) const
{
	return (m_autoSaveLastSchemeChBox) ? (m_autoSaveLastSchemeChBox->checkState() == Qt::Checked) : false;
}

void COptionsWindow::setAutoSaveLastScheme(const bool &autoSaveLastScheme)
{
	if(m_autoSaveLastSchemeChBox) m_autoSaveLastSchemeChBox->setChecked(autoSaveLastScheme);
}

bool COptionsWindow::isAutoLoadLastScheme(void) const
{
	return (m_autoLoadLastSchemeChBox) ? (m_autoLoadLastSchemeChBox->checkState() == Qt::Checked) : false;
}

void COptionsWindow::setAutoLoadLastScheme(const bool &autoLoadLastScheme)
{
	if(m_autoLoadLastSchemeChBox) m_autoLoadLastSchemeChBox->setChecked(autoLoadLastScheme);
}

QColor COptionsWindow::gridColor(void) const
{
	if(m_gridColorLEdit) return m_gridColorLEdit->color();
	return QColor();
}

void COptionsWindow::setGridColor(const QColor &color)
{
	if(m_gridColorLEdit) m_gridColorLEdit->setColor(color);
}

QColor COptionsWindow::gridBkGndColor(void) const
{
	if(m_gridBkGndColorLEdit) return m_gridBkGndColorLEdit->color();
	return QColor();
}

void COptionsWindow::setGridBkGndColor(const QColor &bkGndColor)
{
	if(m_gridBkGndColorLEdit) m_gridBkGndColorLEdit->setColor(bkGndColor);
}

int COptionsWindow::gridStep(void) const
{
	if(m_gridStepSpBox) return m_gridStepSpBox->value();
	return 0;
}

void COptionsWindow::setGridStep(const int &gridStep)
{
	if(m_gridStepSpBox) m_gridStepSpBox->setValue(gridStep);
}

qreal COptionsWindow::gridPointSize(void) const
{
	return m_gridPointSize ? m_gridPointSize->value() : 0.0;
}

void COptionsWindow::setGridPointSize(const qreal &gridPointSize)
{
	if(m_gridPointSize) m_gridPointSize->setValue(gridPointSize);
}

bool COptionsWindow::isGridAlign(void) const
{
	if(m_gridAlignChBox) return m_gridAlignChBox->isChecked();
	return false;
}

void COptionsWindow::setGridAlign(const bool &gridAlign)
{
	if(m_gridAlignChBox) m_gridAlignChBox->setChecked(gridAlign);
}

qreal COptionsWindow::schemeWidth(void) const
{
	if(m_schemeWidthSpBox) return m_schemeWidthSpBox->value();
	return 0.0;
}

void COptionsWindow::setSchemeWidhet(const qreal &schemeWidhet)
{
	if(m_schemeWidthSpBox) m_schemeWidthSpBox->setValue(schemeWidhet);
}

qreal COptionsWindow::schemeHeight(void) const
{
	if(m_schemeHeightSpBox) return m_schemeHeightSpBox->value();
	return 0.0;
}

void COptionsWindow::setSchemeHeight(const qreal &schemeHeight)
{
	if(m_schemeHeightSpBox) m_schemeHeightSpBox->setValue(schemeHeight);
}

qreal COptionsWindow::startTime(void) const
{
	return m_startTimeSpBox != 0 ? m_startTimeSpBox->value() : 0.0;
}

void COptionsWindow::setStartTime(const qreal &startTime)
{
	if(m_startTimeSpBox) m_startTimeSpBox->setValue(startTime);
}

qreal COptionsWindow::timeStep(void) const
{
	return m_timeStepSpBox != 0 ? m_timeStepSpBox->value() : 0.0;
}

void COptionsWindow::setTimeStep(const qreal &timeStep)
{
	if(m_timeStepSpBox) m_timeStepSpBox->setValue(timeStep);
}

qreal COptionsWindow::endTime(void) const
{
	return m_endTimeSpBox != 0 ? m_endTimeSpBox->value() : 0.0;
}

void COptionsWindow::setEndTime(const qreal &endTime)
{
	if(m_endTimeSpBox) m_endTimeSpBox->setValue(endTime);
}

void COptionsWindow::accept(void)
{
	if(m_startTimeSpBox && m_timeStepSpBox && m_endTimeSpBox)
	{
		QMessageBox msgBox;
		if(m_startTimeSpBox->value() > m_endTimeSpBox->value())
		{
			msgBox.setText(tr("Parametr ""start time"" > ""end time"""));
			msgBox.exec();
			m_startTimeSpBox->activateWindow();
			return;
		}
		if(m_timeStepSpBox->value() == 0)
		{
			msgBox.setText(tr("Parametr ""time step"" = 0"));
			msgBox.exec();
			m_timeStepSpBox->activateWindow();
			return;
		}
	}

	QDialog::accept();
}