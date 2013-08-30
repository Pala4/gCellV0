#include "csum.h"

#include <QAction>

#include "../../../scheme/portal/cargument.h"
#include "../../../scheme/portal/cresult.h"

/*!
 * \class CSummatorArgument
 */
CSumArgument::CSumArgument(QGraphicsItem *parent) : CArgument(parent)
{
	setObjectName(QStringLiteral("CSummatorArgument"));

	m_invert = false;

	QAction *acInvers = new QAction(tr("Invert"), this);
	acInvers->setObjectName(QStringLiteral("acInvers"));
	acInvers->setCheckable(true);
	acInvers->setChecked(false);
	connect(acInvers, SIGNAL(triggered(bool)), this, SLOT(setInvert(bool)));
	addAction(acInvers);
}

stData CSumArgument::bufferData(const quint64 &timeFrameIndex)
{
	stData bufData = CArgument::bufferData(timeFrameIndex);
	if(isInvert()) bufData.value = -bufData.value;
	return bufData;
}

void CSumArgument::setInvert(const bool &invert)
{
	if(m_invert == invert) return;
	m_invert = invert;
	QString prefix = captionPrefix();
	prefix.remove(QChar('-'));
	if(m_invert)
	{
		prefix += QChar('-');
	}
	setCaptionPrefix(prefix);
}

/*!
 * \class CSum
 */
void CSum::proced(const stTimeLine &timeLine)
{
	qreal sum = 0;
	foreach(CSumArgument *sumArg, m_sumArgs)
	{
		if(sumArg) sum += sumArg->bufferData(timeLine.timeFrame.timeFrameIndex).value;
	}
	m_sumRes->appendBuffer(timeLine.timeFrame, sum);
}

CSum::CSum(QGraphicsItem *parent) : CDataTransmitter(parent)
{
	setObjectName(QStringLiteral("CSum"));

	m_sumRes = 0;

	CSumArgument *inArg = addSumArg();
	inArg->setName(tr("In"));

	CSumArgument *x1Arg = addSumArg();
	x1Arg->setName(QStringLiteral("x1"));
	x1Arg->setPortalOrientation(CPortal::Top);

	CSumArgument *x2Arg = addSumArg();
	x2Arg->setName(QStringLiteral("x2"));
	x2Arg->setPortalOrientation(CPortal::Bottom);

	m_sumRes = addResult("Sum");

	QAction *acAddSumArg = new QAction(tr("Add argument"), this);
	acAddSumArg->setObjectName(QStringLiteral("acAddSumArg"));
	connect(acAddSumArg, SIGNAL(triggered()), this, SLOT(addSumArg()));
	addAction(acAddSumArg);
}

CElement* CSum::createElement(const QString &typeID)
{
	if(typeID == CSumArgument::staticMetaObject.className())
	{
		return addSumArg();
	}
	return CDataTransmitter::createElement(typeID);
}

CSumArgument* CSum::addSumArg(void)
{
	 CSumArgument *sumArg = new CSumArgument(this);
	 addArgument(sumArg);
	 m_sumArgs << sumArg;
	 return sumArg;
}