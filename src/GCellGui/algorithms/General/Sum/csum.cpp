#include "csum.h"

#include <QAction>

#include "portal/cargument.h"
#include "portal/cresult.h"

/*!
 * \class CSummatorArgument
 */
CSumArgument::CSumArgument(QGraphicsItem *parent) : CArgument(parent)
{
	setObjectName(QStringLiteral("CSummatorArgument"));

    m_bInvert = false;

	QAction *acInvers = new QAction(tr("Invert"), this);
	acInvers->setObjectName(QStringLiteral("acInvers"));
	acInvers->setCheckable(true);
	acInvers->setChecked(false);
	connect(acInvers, SIGNAL(triggered(bool)), this, SLOT(setInvert(bool)));
	addAction(acInvers);

	setInteractions(intercations() | CElement::Deletable);
}

stData CSumArgument::bufferData(const unsigned long long &ullTFIndex)
{
    stData bufData = CArgument::bufferData(ullTFIndex);
    if (isInvert())
        bufData.ldblValue = -bufData.ldblValue;

	return bufData;
}

void CSumArgument::setInvert(const bool &invert)
{
    if (m_bInvert == invert)
        return;

    m_bInvert = invert;
	QString prefix = captionPrefix();
	prefix.remove(QChar('-'));
    if (m_bInvert)
		prefix += QChar('-');

    setCaptionPrefix(prefix);
}

/*!
 * \class CSum
 */
void CSum::proced(const unsigned long long &ullTFIndex, const long double &ldblTimeFrame,
                  const long double &ldblStartTime, const long double &ldblTimeStep,
                  const long double &ldblEndTime)
{
    Q_UNUSED(ldblStartTime)
    Q_UNUSED(ldblTimeStep)
    Q_UNUSED(ldblEndTime)

    long double ldblSum = 0.0L;
    foreach (CSumArgument *sumArg, m_sumArgs) {
        if (sumArg != nullptr)
            ldblSum += sumArg->bufferData(ullTFIndex).ldblValue;
	}

    m_pSumRes->appendBuffer(ldblTimeFrame, ldblSum);
}

CSum::CSum(QGraphicsItem *parent) : CDataTransmitter(parent)
{
	setObjectName(QStringLiteral("CSum"));

    m_pSumRes = nullptr;

	CSumArgument *inArg = addSumArg();
	inArg->setName(tr("In"));

	CSumArgument *x1Arg = addSumArg();
	x1Arg->setName(QStringLiteral("x1"));
    x1Arg->setPortalOrientation(CPortal::Top);

	CSumArgument *x2Arg = addSumArg();
	x2Arg->setName(QStringLiteral("x2"));
    x2Arg->setPortalOrientation(CPortal::Bottom);

    m_pSumRes = addResult("Sum");

	QAction *acAddSumArg = new QAction(tr("Add argument"), this);
	acAddSumArg->setObjectName(QStringLiteral("acAddSumArg"));
	connect(acAddSumArg, SIGNAL(triggered()), this, SLOT(addSumArg()));
	addAction(acAddSumArg);
}

CElement* CSum::createElement(const QString &typeID)
{
    if (typeID == CSumArgument::staticMetaObject.className())
		return addSumArg();

	return CDataTransmitter::createElement(typeID);
}

CSumArgument* CSum::addSumArg()
{
	 CSumArgument *sumArg = new CSumArgument(this);
	 addArgument(sumArg);
	 connect(sumArg, SIGNAL(destroyed(QObject*)), this, SLOT(onSumArgDestroyed(QObject*)));
     m_sumArgs << sumArg;

	 return sumArg;
}

void CSum::onSumArgDestroyed(QObject *objSumArg)
{
    CSumArgument *sumArg = reinterpret_cast<CSumArgument*>(objSumArg);
    if ((sumArg != nullptr) && m_sumArgs.contains(sumArg))
        m_sumArgs.removeOne(sumArg);
}