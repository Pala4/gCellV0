#ifndef CSTEPEXCITATION_H
#define CSTEPEXCITATION_H

#include "algorithm/cdatasource.h"

class CResult;

class CStepExcitation : public CDataSource
{
	Q_OBJECT
private:
    CResult *m_pResStep;
protected:
    void proced(const unsigned long long &ullTFIndex, const long double &ldblTimeFrame,
                const long double &ldblStartTime, const long double &ldblTimeStep,
                const long double &ldblEndTime);
public:
    Q_INVOKABLE explicit CStepExcitation(QGraphicsItem *parent = nullptr);
};

#endif // CSTEPEXCITATION_H