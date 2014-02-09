#include "ciosystem.h"

#include "cchannel.h"

int CIOSystem::generateChannelID()
{
    int maxID = -2;
    foreach (CChannel *channel, m_channels) {
        if (channel == nullptr)
            continue;

        if (channel->id() > maxID)
            maxID = channel->id();
    }

    if (maxID < 0)
        maxID = -1;

    return ++maxID;
}

CIOSystem::CIOSystem(QObject *parent) : QObject(parent)
{
    setObjectName(QStringLiteral("CIOSystem"));
}

CChannel* CIOSystem::createChannel()
{
    CChannel *channel = new CChannel(generateChannelID(), this);
    channel->setObjectName(QString("Channel#%1").arg(channel->id()));
    connect(channel, SIGNAL(forwardCmd(int,QString)), this, SLOT(sendForwardCmd(int,QString)));
    connect(channel, SIGNAL(forwardMsg(int,QString)), this, SLOT(sendForwardMsg(int,QString)));
    m_channels[channel->id()] = channel;
    return channel;
}

void CIOSystem::onChannelDestroyed(QObject *objChannel)
{
    CChannel *channel = reinterpret_cast<CChannel*>(objChannel);
    if (m_channels.values().contains(channel))
        m_channels.remove(m_channels.key(channel));
}

void CIOSystem::sendBackwardCmd(const int &channelID, const QString &cmd)
{
    if (m_channels.contains(channelID) && (m_channels[channelID] != nullptr)
        && (m_channels[channelID]->id() == channelID)) {
        m_channels[channelID]->sendBackwardCmd(cmd);
    }

    emit backwardCmd(cmd);
}

void CIOSystem::sendBackwardMsg(const int &channelID, const QString &msg)
{
    if (m_channels.contains(channelID) && (m_channels[channelID] != nullptr)
        && (m_channels[channelID]->id() == channelID)) {
        m_channels[channelID]->sendBackwardMsg(msg);
    }

    emit backwardMsg(msg);
}

void CIOSystem::sendForwardCmd(const int &channelID, const QString &cmd)
{
    if (cmd == "GetIOSystemInfo") {
        sendBackwardMsg(channelID, QString("IO System v0.0.1 initialized"));
        return;
    }

    emit forwardCmd(cmd);
}

void CIOSystem::sendForwardMsg(const int &channelID, const QString &msg)
{
    Q_UNUSED(channelID)
    emit forwardMsg(msg);
}