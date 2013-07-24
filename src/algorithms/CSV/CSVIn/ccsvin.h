#ifndef CCSVIN_H
#define CCSVIN_H

#include "../../../scheme/cdatasource.h"

class CCSVIn : public CDataSource
{
    Q_OBJECT
private:
	CArgument *arg1;
	CResult *res1;
	CResult *res2;
protected:
	virtual void proced(const int &timeFrame);
public:
	Q_INVOKABLE explicit CCSVIn(QGraphicsItem *parent = 0);
};

#endif // CCSVIN_H