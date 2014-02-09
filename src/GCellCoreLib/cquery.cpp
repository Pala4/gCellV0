#include "cquery.h"

CQuery::CQuery(QObject *parent) : QObject(parent)
{
    setObjectName(QStringLiteral("CQuery"));
}