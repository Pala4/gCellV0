#include "ctransaction.h"

#include <QCoreApplication>

#include "ctransactionevent.h"

void CTransaction::setTransactionType(const CTransaction::TransactionType &transactionType)
{
    m_transactionType = transactionType;
}

CTransaction::CTransaction(QObject *queryReceiver, QObject *responsReceiver, const QString &query,
                           const int &cmdID, QObject *parent) : QObject(parent)
{
    setObjectName(QStringLiteral("CTranzaction"));

    m_transactionType = CTransaction::Query;
    m_queryReceiver = nullptr;
    m_responsReceiver = nullptr;
    m_query = query;
    m_cmdID = cmdID;

    setQueryReceiver(queryReceiver);
    setResponsReceiver(responsReceiver);
}

void CTransaction::setQueryReceiver(QObject *queryReceiver)
{
    if ((m_queryReceiver != nullptr) && (m_queryReceiver == queryReceiver))
        return;
    if (m_queryReceiver != nullptr)
        disconnect(m_queryReceiver, SIGNAL(destroyed()), this, SLOT(onQueryReceiverDestroyed()));
    m_queryReceiver = queryReceiver;
    if (m_queryReceiver != nullptr)
        connect(m_queryReceiver, SIGNAL(destroyed()), this, SLOT(onQueryReceiverDestroyed()));
}

void CTransaction::setResponsReceiver(QObject *responsReceiver)
{
    if ((m_responsReceiver != nullptr) && (m_queryReceiver == responsReceiver))
        return;
    if (m_responsReceiver != nullptr) {
        disconnect(m_responsReceiver, SIGNAL(destroyed()),
                   this, SLOT(onResponsReceiverDestroyed()));
    }
    m_responsReceiver = responsReceiver;
    if (m_responsReceiver != nullptr)
        connect(m_responsReceiver, SIGNAL(destroyed()), this, SLOT(onResponsReceiverDestroyed()));
}

void CTransaction::sendQuery()
{
    if (m_queryReceiver != nullptr) {
        setTransactionType(CTransaction::Query);
        QCoreApplication::postEvent(m_queryReceiver, new CTransactionEvent(this));
    }
}

void CTransaction::sendRespons()
{
    if (m_responsReceiver != nullptr) {
        setTransactionType(CTransaction::Respons);
        QCoreApplication::postEvent(m_responsReceiver, new CTransactionEvent(this));
    }
}

void CTransaction::sendRespons(const QString &respons)
{
    setRespons(respons);
    sendRespons();
}
