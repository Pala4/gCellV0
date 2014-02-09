#include "cchannel.h"

CChannel::CChannel(const int &id, QObject *parent) : QObject(parent)
{
    setObjectName("CChannel");

    setID(id);
}

void CChannel::sendBackwardCmd(const QString &cmd)
{
    emit backwardCmd(cmd);
}

void CChannel::sendBackwardMsg(const QString &msg)
{
    emit backwardMsg(msg);
}

void CChannel::sendForwardCmd(const QString &cmd)
{
    if (cmd == "GetChannelInfo") {
        sendBackwardMsg(QString("Channel id #%1").arg(id()));
        return;
    }

    emit forwardCmd(id(), cmd);
}

void CChannel::sendForwardMsg(const QString &msg)
{
    emit forwardMsg(id(), msg);
}