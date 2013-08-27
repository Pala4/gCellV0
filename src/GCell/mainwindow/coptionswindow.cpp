#include "coptionswindow.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QMessageBox>

COptionsWindow::COptionsWindow(QWidget *parent) : QDialog(parent)
{
	setObjectName(QStringLiteral("COptionsWindow"));

	QVBoxLayout *vbLayout = new QVBoxLayout();
	setLayout(vbLayout);

	QFormLayout *flTimeLine = new QFormLayout();
	vbLayout->addLayout(flTimeLine);

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

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
													   Qt::Horizontal);
	vbLayout->addWidget(buttonBox);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
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