#include "ctranslink.h"

#include <QFormLayout>
#include <QComboBox>
#include <QLineEdit>

#include "celementoptionswgt.h"
#include "portal/cargument.h"
#include "portal/cresult.h"
#include "cdiscretdiffmethod.h"

QVector<qreal> CTransLink::stringToVector(const QString &string, const QString &sep)
{
	QVector<qreal> resVector;
	QStringList strList = string.split(sep);
    if (strList.isEmpty())
        return resVector;

    foreach (QString str, strList) {
		bool ok = false;
		qreal factor = str.toDouble(&ok);
        if (ok)
            resVector << factor;
	}

	return resVector;
}

QString CTransLink::vectorToString(const QVector<qreal> &vector, const QString &sep)
{
	QStringList strList;

    foreach (qreal factor, vector)
        strList << QString("%1").arg(factor);

	return strList.join(sep);
}

void CTransLink::proced(const unsigned long long &ullTFIndex, const long double &ldblTimeFrame,
                        const long double &ldblStartTime, const long double &ldblTimeStep,
                        const long double &ldblEndTime)
{
    Q_UNUSED(ullTFIndex)
    Q_UNUSED(ldblStartTime)
    Q_UNUSED(ldblTimeStep)
    Q_UNUSED(ldblEndTime)

    if ((selectedTLAlg() < 0) || (selectedTLAlg() >= m_tlAlgs.count()))
        return;
    if (m_tlAlgs.at(selectedTLAlg()) == nullptr)
        return;

    stData resData =  m_tlAlgs.at(selectedTLAlg())->calculate(ullTFIndex, ldblTimeFrame,
                                                              ldblStartTime, ldblTimeStep,
                                                              ldblEndTime,
                                                              m_pInArg->buffer()->data(),
															  m_pOutRes->buffer()->data());
    m_pOutRes->appendBuffer(ldblTimeFrame, resData.ldblValue);
}

CTransLink::CTransLink(QGraphicsItem *parent) : CDataTransmitter(parent)
{
	setObjectName(QStringLiteral("CTransLink"));

    m_pInArg = nullptr;
    m_pOutRes = nullptr;

    m_selTLAlgComboBox = nullptr;
    m_nominatorFactorsLineEdit = nullptr;
    m_denominatorFactorsLineEdit = nullptr;
	m_selectedTLAlg = -1;
	m_factorSeparator = QChar(';');

	m_pInArg = addArgument(tr("In"));
	m_pOutRes = addResult(tr("Out"));

	m_nomFactors << 0.8;
	m_denomFactors << 1.0 << 0.7 << 0.08;

    CDiscretDiffMethod *ddmTLAlg = new CDiscretDiffMethod(tr("Method of discrete differences"),
                                                          &m_nomFactors, &m_denomFactors);
	m_tlAlgs << ddmTLAlg;
	setSelectedTLAlg(0);
}

CTransLink::~CTransLink()
{
    foreach (CTLAbstractAlgorithm *tlAlg, m_tlAlgs) {
        if (tlAlg != nullptr)
            delete tlAlg;
	}
}

void CTransLink::setSelectedTLAlg(const int &selectedTLAlg)
{
    if (m_selectedTLAlg == selectedTLAlg)
        return;
    if ((selectedTLAlg < 0) || (selectedTLAlg >= m_tlAlgs.count()))
        return;

	m_selectedTLAlg = selectedTLAlg;
}

QString CTransLink::nomFactorsStr()
{
	return vectorToString(m_nomFactors, factorSeparator());
}

void CTransLink::setNomFactorsStr(const QString &poles)
{
	setNomFactors(stringToVector(poles, factorSeparator()));
}

QString CTransLink::denomFactorsStr()
{
	return vectorToString(m_denomFactors, factorSeparator());
}

void CTransLink::setDenomFactorsStr(const QString &nulls)
{
	setDenomFactors(stringToVector(nulls, factorSeparator()));
}

void CTransLink::setNomFactors(const QVector<qreal> &nomFactors)
{
    if (m_nomFactors == nomFactors)
        return;

    m_nomFactors = nomFactors;
	emit nomFactorsChanged(m_nomFactors);
}

void CTransLink::setDenomFactors(const QVector<qreal> &denomFactors)
{
    if (m_denomFactors == denomFactors)
        return;

	m_denomFactors = denomFactors;
	emit denomFactorsChanged(m_denomFactors);
}

CElementOptionsWgt* CTransLink::optionsWidget(QWidget *parentWidget)
{
	CElementOptionsWgt *optWgt = CDataTransmitter::optionsWidget(parentWidget);
    if (optWgt != nullptr) {
		m_selTLAlgComboBox = new QComboBox();
        for (int algIndex = 0; algIndex < m_tlAlgs.count(); ++algIndex) {
			CTLAbstractAlgorithm *tlAlg = m_tlAlgs.at(algIndex);
            if (tlAlg != nullptr)
                m_selTLAlgComboBox->addItem(tlAlg->methodName());
		}
        if (m_selTLAlgComboBox->count() > selectedTLAlg())
			m_selTLAlgComboBox->setCurrentIndex(selectedTLAlg());

		optWgt->generalFormLayout()->addRow(tr("Transformation algorithm:"), m_selTLAlgComboBox);

		m_nominatorFactorsLineEdit = new QLineEdit(nomFactorsStr());
        optWgt->generalFormLayout()->addRow(tr("Nominator factors (A0;A1;...;An):"),
                                            m_nominatorFactorsLineEdit);
		m_denominatorFactorsLineEdit = new QLineEdit(denomFactorsStr());
        optWgt->generalFormLayout()->addRow(tr("Denominator factors (B0;B1;...;Bm):"),
                                            m_denominatorFactorsLineEdit);
	}

	return optWgt;
}

void CTransLink::acceptOptions(CElementOptionsWgt *optWgt)
{
	CDataTransmitter::acceptOptions(optWgt);

    if (m_selTLAlgComboBox != nullptr)
		setSelectedTLAlg(m_selTLAlgComboBox->currentIndex());
    if (m_nominatorFactorsLineEdit)
        setNomFactorsStr(m_nominatorFactorsLineEdit->text());
    if (m_denominatorFactorsLineEdit)
        setDenomFactorsStr(m_denominatorFactorsLineEdit->text());
}

void CTransLink::beforeCalc(const long double &ldblStartTime, const long double &ldblTimeStep,
                            const long double &ldblEndTime)
{
    if ((selectedTLAlg() >= 0) && (selectedTLAlg() < m_tlAlgs.count())) {
        if (m_tlAlgs.at(selectedTLAlg()))
            m_tlAlgs.at(selectedTLAlg())->update(ldblStartTime, ldblTimeStep, ldblEndTime);
	}
}