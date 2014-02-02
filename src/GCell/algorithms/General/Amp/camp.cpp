#include "camp.h"

#include <QDoubleSpinBox>
#include <QFormLayout>

#include "celementoptionswgt.h"
#include "portal/cargument.h"
#include "portal/cresult.h"

void CAmp::proced(const unsigned long long &ullTFIndex, const long double &ldblTimeFrame,
                  const long double &ldblStartTime, const long double &ldblTimeStep,
                  const long double &ldblEndTime)
{
    Q_UNUSED(ldblStartTime)
    Q_UNUSED(ldblTimeStep)
    Q_UNUSED(ldblEndTime)

    if ((m_arg != nullptr) && (m_res != nullptr))
        m_res->appendBuffer(ldblTimeFrame, m_arg->bufferData(ullTFIndex).ldblValue*m_gain);
}

CAmp::CAmp(QGraphicsItem *parent) : CDataTransmitter(parent)
{
    setObjectName(QStringLiteral("CAmp"));

	m_gain = 1.0;
    m_arg = nullptr;
    m_res = nullptr;
    m_gainSpinBox = nullptr;

	m_arg = addArgument("X");
	m_res = addResult("Y");
}

CElementOptionsWgt* CAmp::optionsWidget(QWidget *parentWidget)
{
	CElementOptionsWgt *optWgt = CDataTransmitter::optionsWidget(parentWidget);
    if (optWgt != nullptr) {
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

    if (m_gainSpinBox != nullptr)
		m_gain = m_gainSpinBox->value();
}