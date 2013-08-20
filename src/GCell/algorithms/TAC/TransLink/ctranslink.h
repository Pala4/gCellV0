#ifndef CTRANSLINK_H
#define CTRANSLINK_H

#include "../../../scheme/algorithm/cdatatransmitter.h"

class CArgument;
class CResult;

class CTransLink : public CDataTransmitter
{
	Q_OBJECT
private:
	CArgument *m_inArg;
	CResult *m_outRes;
protected:
	virtual void proced(const int &timeFrame);
public:
	Q_INVOKABLE explicit CTransLink(QGraphicsItem *parent = 0);
};

#endif // CTRANSLINK_H