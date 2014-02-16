#include "cobject.h"

#include "transaction/ctransactionevent.h"
#include "transaction/ctransaction.h"

void CObject::processTransactionQuery(CTransaction *transaction)
{
    Q_UNUSED(transaction)
}

void CObject::processTransactionRespons(CTransaction *transaction)
{
    Q_UNUSED(transaction)
}

bool CObject::event(QEvent *event)
{
    if (static_cast<int>(event->type()) == static_cast<int>(CTransactionEvent::TransactionEvent)) {
        CTransactionEvent *transactionEvent = dynamic_cast<CTransactionEvent*>(event);
        if (transactionEvent != nullptr) {
            if (transactionEvent->transaction() != nullptr) {
                if (transactionEvent->transaction()->isQuery())
                    processTransactionQuery(transactionEvent->transaction());
                else
                    processTransactionRespons(transactionEvent->transaction());
                return true;
            }
        }
    }

    return QObject::event(event);
}

bool CObject::processBackwardQuery(const QString &backwardQuery)
{
    Q_UNUSED(backwardQuery)
    return false;
}

bool CObject::processBackwardRespons(const QString &backwardRespons)
{
    Q_UNUSED(backwardRespons)
    return false;
}

bool CObject::processForwardQuery(const QString &forwardQuery)
{
    Q_UNUSED(forwardQuery)
    return false;
}

bool CObject::processForwardRespons(const QString &forwardRespons)
{
    Q_UNUSED(forwardRespons)
    return false;
}

CObject::CObject(QObject *parent) : QObject(parent)
{
    setObjectName(QStringLiteral("CObject"));
}

void CObject::receiveBackwardQuery(const QString &query)
{
    if (!processBackwardQuery(query))
        emit transmitBackwardQuery(query);
}

void CObject::receiveBackwardRespons(const QString &respons)
{
    if (!processBackwardRespons(respons))
        emit transmitBackwardRespons(respons);
}

void CObject::receiveForwardQuery(const QString &query)
{
    if (!processForwardQuery(query))
        emit transmitForwardQuery(query);
}

void CObject::receiveForwardRespons(const QString &respons)
{
    if (!processForwardRespons(respons))
        emit transmitForwardRespons(respons);
}