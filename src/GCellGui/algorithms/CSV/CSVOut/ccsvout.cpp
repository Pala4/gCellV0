#include "ccsvout.h"

#include "../../../scheme/portal/cargument.h"

void CCSVOut::proced(const unsigned long long &ullTFIndex, const long double &ldblTimeFrame,
                     const long double &ldblStartTime, const long double &ldblTimeStep,
                     const long double &ldblEndTime)
{
    Q_UNUSED(ullTFIndex)
    Q_UNUSED(ldblTimeFrame)
    Q_UNUSED(ldblStartTime)
    Q_UNUSED(ldblTimeStep)
    Q_UNUSED(ldblEndTime)
}

CCSVOut::CCSVOut(QGraphicsItem *parent) : CDataReceiver(parent)
{
    setObjectName(QStringLiteral("CCSVOut"));

    addArgument("X");
}