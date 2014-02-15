#include "cchannel.h"

CChannel::CChannel(const int &id, QObject *parent) : QObject(parent)
{
    setObjectName("CChannel");

    setID(id);
}

void CChannel::sendBackwardQuery(const QString &query)
{
    emit backwardQuery(query);
}

void CChannel::sendBackwardRespons(const QString &respons)
{
    emit backwardRespons(respons);
}

void CChannel::sendForwardQuery(const QString &query)
{
    emit forwardQuery(id(), query);
}

void CChannel::sendForwardRespons(const QString &respons)
{
    emit forwardRespons(id(), respons);
}
