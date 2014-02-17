#include "ciosystem.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>

#include "ctransaction.h"
#include "cqueryparser.h"

bool CIOSystem::processForwardQuery(const QString &forwardQuery)
{
    bool error = false;
    QString errorDescription;
    QStringList queryList = CQueryParser::pars(forwardQuery, error, errorDescription);
    if (error) {
        receiveBackwardRespons(errorDescription);
        return true;
    }
    if (queryList.isEmpty())
        return true;

    QString queryCmd = queryList.at(0);
    QStringList argList;
    if (queryList.count() > 1)
        argList = queryList.mid(1, queryList.count() - 1);
    if (!queryCmd.isEmpty() && m_transactions.contains(queryCmd)
        && (m_transactions[queryCmd] != nullptr)) {
        m_transactions[queryCmd]->sendQuery(argList);
        return true;
    }

    return false;
}

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