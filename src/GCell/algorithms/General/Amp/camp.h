#ifndef CAMP_H
#define CAMP_H

#include "../../../scheme/algorithm/cdatatransmitter.h"

class CAmp : public CDataTransmitter
{
    Q_OBJECT
private:
	CArgument *arg;
	CResult *res;
protected:
	virtual void proced(const int &timeFrame);
public:
    Q_INVOKABLE explicit CAmp(QGraphicsItem *parent = 0);
};

#endif // CAMP_H