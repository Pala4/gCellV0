#ifndef CSUM_H
#define CSUM_H

#include "../../../scheme/algorithm/cdatatransmitter.h"

class CSum : public CDataTransmitter
{
    Q_OBJECT
private:
    CArgument *m_inArg;
    CArgument *m_x1Arg;
    CArgument *m_x2Arg;
    CResult *m_sumRes;
protected:
	virtual void proced(const stTimeLine &timeLine);
public:
    Q_INVOKABLE explicit CSum(QGraphicsItem *parent = 0);
};

#endif // CSUM_H