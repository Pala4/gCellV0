#include "cthreadmng.h"

CThreadMng::CThreadMng(QObject *parent) : QObject(parent)
{
    setObjectName(QStringLiteral("CThreadMng"));
}
