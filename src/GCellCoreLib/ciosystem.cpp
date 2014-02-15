#include "ciosystem.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>

#include "cchannel.h"
#include "ctransaction.h"
#include "ctransactionevent.h"

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

void CIOSystem::processTransaction(CTransaction *transaction)
{
    if (transaction == nullptr)
        return;

    if (transaction->isRespons())
        sendBackwardRespons(transaction->channelID(), transaction->respons());
}

CIOSystem::CIOSystem(QObject *parent) : QObject(parent), CBase()
{
    setObjectName(QStringLiteral("CIOSystem"));
    initTransactionProcessor();
}

CTransaction* CIOSystem::registerTransaction(QObject *queryReceiver, const QString &query,
                                             const int &cmdID)
{
    if ((queryReceiver == nullptr) || query.isEmpty() || (cmdID < 0))
        return nullptr;
    if (m_transactions.contains(query)) {
        qWarning(qPrintable(QString("Query descriptor with name "
                                    "[%1] already exist").arg(query)));
        return nullptr;
    }

    CTransaction *transaction = new CTransaction(queryReceiver, this, query, cmdID, this);
    m_transactions[query] = transaction;
    connect(transaction, SIGNAL(destroyed(QObject*)),
            this, SLOT(onTransactionDestroyed(QObject*)));

    return transaction;
}

CChannel* CIOSystem::createChannel()
{
    CChannel *channel = new CChannel(generateChannelID(), this);
    channel->setObjectName(QString("Channel#%1").arg(channel->id()));
    connect(channel, SIGNAL(forwardQuery(int,QString)), this, SLOT(sendForwardQuery(int,QString)));
    connect(channel, SIGNAL(forwardRespons(int,QString)),
            this, SLOT(sendForwardRespons(int,QString)));
    m_channels[channel->id()] = channel;
    return channel;
}

void CIOSystem::onChannelDestroyed(QObject *objChannel)
{
    CChannel *channel = reinterpret_cast<CChannel*>(objChannel);
    if (m_channels.values().contains(channel))
        m_channels.remove(m_channels.key(channel));
}

void CIOSystem::onTransactionDestroyed(QObject *objTransaction)
{
    CTransaction *transaction = reinterpret_cast<CTransaction*>(objTransaction);
    if (m_transactions.values().contains(transaction))
        m_transactions.remove(m_transactions.key(transaction));
}


void CIOSystem::sendBackwardQuery(const int &channelID, const QString &query)
{
    if (!query.isEmpty() && m_channels.contains(channelID) && (m_channels[channelID] != nullptr)
        && (m_channels[channelID]->id() == channelID)) {
        m_channels[channelID]->sendBackwardQuery(query);
    }

    emit backwardQuery(query);
}

void CIOSystem::sendBackwardRespons(const int &channelID, const QString &respons)
{
    if (!respons.isEmpty() && m_channels.contains(channelID) && (m_channels[channelID] != nullptr)
        && (m_channels[channelID]->id() == channelID)) {
        m_channels[channelID]->sendBackwardRespons(respons);
    }

    emit backwardRespons(respons);
}

void CIOSystem::sendForwardQuery(const int &channelID, const QString &query)
{
    if (m_transactions.contains(query) && (m_transactions[query] != nullptr)) {
        m_transactions[query]->setChannelID(channelID);
        m_transactions[query]->sendQuery();
    }

    emit forwardQuery();
}

void CIOSystem::sendForwardRespons(const int &channelID, const QString &respons)
{
    emit forwardRespons();
}
