#ifndef CSTEPEXCITATION_H
#define CSTEPEXCITATION_H

#include "../../../scheme/algorithm/cdatasource.h"

class CResult;

class CStepExcitation : public CDataSource
{
	Q_OBJECT
private:
	CResult *m_step;
protected:
	virtual void proced(const int &timeFrame);
public:
	Q_INVOKABLE explicit CStepExcitation(QGraphicsItem *parent = 0);
};

#endif // CSTEPEXCITATION_H