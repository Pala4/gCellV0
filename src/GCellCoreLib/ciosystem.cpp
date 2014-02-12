#include "ciosystem.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>

#include "ccmdevent.h"
#include "cchannel.h"

void CIOSystem::execCommand(const QString &cmdName)
{
    if (cmdName.isEmpty() || (!cmdName.isEmpty() && !m_cmdDescs.contains(cmdName)))
        return;
    CmdDesc cmdDesc = m_cmdDescs[cmdName];
    if ((cmdDesc.receiver == nullptr) || (cmdDesc.cmdID == -1))
        return;

    CCmdEvent cmdEvent(cmdDesc.cmdID);
    QCoreApplication::postEvent(cmdDesc.receiver, &cmdEvent);
}

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

CIOSystem::CIOSystem(QObject *parent) : QObject(parent), CBase()
{
    setObjectName(QStringLiteral("CIOSystem"));
    initCmdEventProcessor();
}

CmdDesc CIOSystem::registerCommand(QObject *receiver, const QString &cmdName, const int &cmdID)
{
    if ((receiver == nullptr) || cmdName.isEmpty() || (cmdID == -1))
        return CmdDesc();
    if (m_cmdDescs.contains(cmdName)) {
        qWarning(qPrintable(QString("Command [%1] already exist").arg(cmdName)));
        return CmdDesc();
    }

    CmdDesc cmdDesc(receiver, cmdName, cmdID);
    m_cmdDescs[cmdName] = cmdDesc;
    connect(receiver, SIGNAL(destroyed(QObject*)), this, SLOT(onCmdReceiverDestroyed(QObject*)));

    return cmdDesc;
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

void CIOSystem::onCmdReceiverDestroyed(QObject *objReceiver)
{
    QStringList remCommands;
    for (int ci = 0; ci < m_cmdDescs.count(); ++ci) {
        if (m_cmdDescs.values().at(ci).receiver == objReceiver)
            remCommands << m_cmdDescs.keys().at(ci);
    }

    foreach (QString cmd, remCommands)
        m_cmdDescs.remove(cmd);
}

void CIOSystem::onChannelDestroyed(QObject *objChannel)
{
    CChannel *channel = reinterpret_cast<CChannel*>(objChannel);
    if (m_channels.values().contains(channel))
        m_channels.remove(m_channels.key(channel));
}

void CIOSystem::sendBackwardCmd(Package pkg)
{
    if ((!pkg.query.isNull()) && m_channels.contains(pkg.channelID)
        && (m_channels[pkg.channelID] != nullptr)
        && (m_channels[pkg.channelID]->id() == pkg.channelID)) {
        m_channels[pkg.channelID]->sendBackwardCmd(/*pkg.query.data()*/pkg.respons);
    }

    emit backwardCmd(/*pkg.query.data()*/pkg.respons);
}

void CIOSystem::sendBackwardMsg(Package pkg)
{
    if (m_channels.contains(pkg.channelID) && (m_channels[pkg.channelID] != nullptr)
        && (m_channels[pkg.channelID]->id() == pkg.channelID)) {
        m_channels[pkg.channelID]->sendBackwardMsg(pkg.respons);
    }

    emit backwardMsg(pkg.respons);
}

void CIOSystem::sendForwardCmd(const int &channelID, const QString &cmd)
{
    if (cmd == "GetIOSystemInfo") {
        Package pkg;
        pkg.channelID = channelID;
        pkg.respons = QString("IO System v0.0.1");
        sendBackwardMsg(pkg);
        return;
    }

    Package pkg;
    pkg.channelID = channelID;
    emit forwardCmd(pkg);
}

void CIOSystem::sendForwardMsg(const int &channelID, const QString &msg)
{
    Package pkg;
    pkg.channelID = channelID;
    pkg.respons = msg;
    emit forwardMsg(pkg);
}