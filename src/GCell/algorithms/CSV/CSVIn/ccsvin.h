#ifndef CCSVIN_H
#define CCSVIN_H

#include "../../../scheme/algorithm/cdatasource.h"

class CCSVIn : public CDataSource
{
    Q_OBJECT
private:
    CResult *m_res1;
    CResult *m_res2;
protected:
	virtual void proced(const int &timeFrame);
public:
	Q_INVOKABLE explicit CCSVIn(QGraphicsItem *parent = 0);
};

#endif // CCSVIN_H