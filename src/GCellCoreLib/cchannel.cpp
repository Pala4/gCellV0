#include "cchannel.h"

CChannel::CChannel(const int &id, QObject *parent) : QObject(parent)
{
    setObjectName("CChannel");

    setID(id);
}

void CChannel::outCmdIn(const QString &cmd)
{
    emit sendCmdIn(cmd);
}

void CChannel::outMsgIn(const QString &msg)
{
    emit sendMsgIn(msg);
}

void CChannel::receiveCmd(const QString &cmd)
{
    if (cmd == "GetChannelInfo") {
        outMsgIn(QString("Channel id #%1").arg(id()));
        return;
    }

    emit sendCmdOut(id(), cmd);
}

void CChannel::receiveMsg(const QString &msg)
{
    emit sendMsgOut(id(), msg);
}