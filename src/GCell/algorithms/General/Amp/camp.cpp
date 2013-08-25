#include "camp.h"

#include <QDoubleSpinBox>
#include <QFormLayout>

#include "../../../scheme/celementoptionswgt.h"
#include "../../../scheme/portal/cargument.h"
#include "../../../scheme/portal/cresult.h"

void CAmp::proced(const stTimeLine &timeLine)
{
	m_res->appendBuffer(timeLine.timeFrame, m_arg->bufferData(timeLine.timeFrame.timeFrameIndex).value*m_gain);
}

CAmp::CAmp(QGraphicsItem *parent) : CDataTransmitter(parent)
{
    setObjectName(QStringLiteral("CAmp"));

	m_gain = 1.0;
	m_arg = 0;
	m_res = 0;
	m_gainSpinBox = 0;

	m_arg = addArgument("X");
	m_res = addResult("Y");
}

CElementOptionsWgt* CAmp::optionsWidget(QWidget *parentWidget)
{
	CElementOptionsWgt *optWgt = CDataTransmitter::optionsWidget(parentWidget);
	if(optWgt)
	{
		m_gainSpinBox = new QDoubleSpinBox();
		m_gainSpinBox->setObjectName(QStringLiteral("gainSpinBox"));
		m_gainSpinBox->setMinimum(-999999.0);
		m_gainSpinBox->setMaximum(999999.0);
		m_gainSpinBox->setSingleStep(0.1);
		m_gainSpinBox->setValue(m_gain);
		optWgt->generalFormLayout()->addRow(tr("Gain"), m_gainSpinBox);
	}
	return optWgt;
}

void CAmp::acceptOptions(CElementOptionsWgt *optWgt)
{
	CDataTransmitter::acceptOptions(optWgt);
	if(m_gainSpinBox)
	{
		m_gain = m_gainSpinBox->value();
	}
}