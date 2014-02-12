#include "ciosystem.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>

#include "cqueryevent.h"
#include "cchannel.h"

void CIOSystem::sendQuery(const QString &queryName)
{
    if (queryName.isEmpty() || (!queryName.isEmpty() && !m_queryDescs.contains(queryName)))
        return;
    QueryDesc queryDesc = m_queryDescs[queryName];
    if ((queryDesc.receiver == nullptr) || (queryDesc.queryID == -1))
        return;

    CQueryEvent queryEvent(queryDesc.queryID);
    QCoreApplication::postEvent(queryDesc.receiver, &queryEvent);
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

QueryDesc CIOSystem::registerCommand(QObject *receiver, const QString &queryName,
                                     const int &queryID)
{
    if ((receiver == nullptr) || queryName.isEmpty() || (queryID == -1))
        return QueryDesc();
    if (m_queryDescs.contains(queryName)) {
        qWarning(qPrintable(QString("Query descriptor with name "
                                    "[%1] already exist").arg(queryName)));
        return QueryDesc();
    }

    QueryDesc queryDesc(receiver, queryName, queryID);
    m_queryDescs[queryName] = queryDesc;
    connect(receiver, SIGNAL(destroyed(QObject*)), this, SLOT(onQueryReceiverDestroyed(QObject*)));

    return queryDesc;
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

void CIOSystem::onQueryReceiverDestroyed(QObject *objReceiver)
{
    QStringList remQueryNames;
    for (int ci = 0; ci < m_queryDescs.count(); ++ci) {
        if (m_queryDescs.values().at(ci).receiver == objReceiver)
            remQueryNames << m_queryDescs.keys().at(ci);
    }

    foreach (QString queryName, remQueryNames)
        m_queryDescs.remove(queryName);
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