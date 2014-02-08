#ifndef CCSVOUT_H
#define CCSVOUT_H

#include "../../../scheme/algorithm/cdatareceiver.h"

class CCSVOut : public CDataReceiver
{
    Q_OBJECT
protected:
    void proced(const unsigned long long &ullTFIndex, const long double &ldblTimeFrame,
                const long double &ldblStartTime, const long double &ldblTimeStep,
                const long double &ldblEndTime);
public:
    Q_INVOKABLE explicit CCSVOut(QGraphicsItem *parent = nullptr);
};

#endif // CCSVOUT_H