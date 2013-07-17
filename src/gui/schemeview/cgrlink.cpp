#include "cgrlink.h"

CGrLink::CGrLink(QGraphicsItem *parent) : QObject(0), QGraphicsPathItem(parent)
{
    setObjectName(QStringLiteral("CGrLink"));
}