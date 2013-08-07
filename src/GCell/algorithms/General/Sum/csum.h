#ifndef CSUM_H
#define CSUM_H

#include "../../../scheme/algorithm/cdatatransmitter.h"

class CSum : public CDataTransmitter
{
    Q_OBJECT
private:
    CArgument *m_arg1;
    CArgument *m_arg2;
    CArgument *m_arg3;
    CResult *m_resSum;
protected:
    virtual void proced(const int &timeFrame);
public:
    Q_INVOKABLE explicit CSum(QGraphicsItem *parent = 0);
};

#endif // CSUM_H