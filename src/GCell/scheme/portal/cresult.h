#ifndef CRESULT_H
#define CRESULT_H

#include "cportal.h"

class CResult : public CPortal
{
	Q_OBJECT
protected:
    QPainterPath calcPortalForm(void);
public:
	explicit CResult(QGraphicsItem *parent = 0);

    void calc(const unsigned long long &ullTFIndex, const long double &ldblTimeFrame,
              const long double &ldblStartTime, const long double &ldblTimeStep,
              const long double &ldblEndTime);
};

#endif // CRESULT_H