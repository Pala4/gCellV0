#ifndef CCSVOUT_H
#define CCSVOUT_H

#include "../../../scheme/cdatareceiver.h"

class CCSVOut : public CDataReceiver
{
    Q_OBJECT
private:
	CArgument *arg;
protected:
	virtual void proced(const int &timeFrame);
public:
    Q_INVOKABLE explicit CCSVOut(QGraphicsItem *parent = 0);
};

#endif // CCSVOUT_H