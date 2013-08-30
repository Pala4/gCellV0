#ifndef CSUM_H
#define CSUM_H

#include "../../../scheme/algorithm/cdatatransmitter.h"
#include "../../../scheme/portal/cargument.h"

class CSumArgument : public CArgument
{
	Q_OBJECT
	Q_PROPERTY(bool invert READ isInvert WRITE setInvert)
private:
	bool m_invert;
public:
	explicit CSumArgument(QGraphicsItem *parent = 0);

	const bool& isInvert(void) const{return m_invert;}
	virtual stData bufferData(const quint64 &timeFrameIndex);
public slots:
	void setInvert(const bool &invert);
};

class CSum : public CDataTransmitter
{
	Q_OBJECT
private:
	QList<CSumArgument*> m_sumArgs;
	CResult *m_sumRes;
protected:
	virtual void proced(const stTimeLine &timeLine);
public:
	Q_INVOKABLE explicit CSum(QGraphicsItem *parent = 0);

	virtual CElement* createElement(const QString &typeID);
private slots:
	CSumArgument* addSumArg(void);
};

#endif // CSUM_H