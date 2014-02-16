#include "ciosystem.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>

#include "ctransaction.h"

CIOSystem::CIOSystem(QObject *parent) : CObject(parent)
{
    setObjectName(QStringLiteral("CIOSystem"));
}

CTransaction* CIOSystem::registerTransaction(QObject *queryReceiver, QObject *responsReceiver,
                                             const QString &query, const int &cmdID)
{
    if (query.isEmpty() || (cmdID < 0))
        return nullptr;
    if (m_transactions.contains(query)) {
        qWarning(qPrintable(QString("Query descriptor with name "
                                    "[%1] already exist").arg(query)));
        return nullptr;
    }

    CTransaction *transaction = new CTransaction(queryReceiver, responsReceiver, query, cmdID,
                                                 this);
    m_transactions[query] = transaction;
    connect(transaction, SIGNAL(destroyed(QObject*)),
            this, SLOT(onTransactionDestroyed(QObject*)));

    return transaction;
}

void CIOSystem::onTransactionDestroyed(QObject *objTransaction)
{
    CTransaction *transaction = reinterpret_cast<CTransaction*>(objTransaction);
    if (m_transactions.values().contains(transaction))
        m_transactions.remove(m_transactions.key(transaction));
}