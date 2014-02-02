#ifndef CARGUMENT_H
#define CARGUMENT_H

#include "cportal.h"

class CArgument : public CPortal
{
	Q_OBJECT
protected:
    QPainterPath calcPortalForm(void);
public:
	explicit CArgument(QGraphicsItem *parent = 0);

    bool canLinked(void){return !isUsed();}
    void calc(const unsigned long long &ullTFIndex, const long double &ldblTimeFrame,
              const long double &ldblStartTime, const long double &ldblTimeStep,
              const long double &ldblEndTime);
};

#endif // CARGUMENT_H