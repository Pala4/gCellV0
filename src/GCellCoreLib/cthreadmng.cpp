#include "cthreadmng.h"

CThreadMng::CThreadMng(QObject *parent) : QObject(parent)
{
    setObjectName(QStringLiteral("CThreadMng"));
}

void CThreadMng::setCmd(const Package &pkg)
{
    Q_UNUSED(pkg)
}

void CThreadMng::setMsg(const Package &pkg)
{
    Q_UNUSED(pkg)
}

void CThreadMng::sendBackwardCmd(const Package &pkg)
{
    emit backwardCmd(pkg);
}

void CThreadMng::sendBackwardMsg(const Package &pkg)
{
    emit backwardMsg(pkg);
}