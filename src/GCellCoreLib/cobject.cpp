#include "cobject.h"

#include "transaction/ctransactionevent.h"

void CObject::processQuery(CTransaction *tranzaction)
{
    Q_UNUSED(tranzaction)
}

void CObject::processRespons(CTransaction *tranzaction)
{
    Q_UNUSED(tranzaction)
}

bool CObject::event(QEvent *event)
{
    if (static_cast<int>(event->type()) == static_cast<int>(CTransactionEvent::TransactionEvent)) {
        CTransactionEvent *transactionEvent = dynamic_cast<CTransactionEvent*>(event);
        if (transactionEvent != nullptr) {
            processQuery(transactionEvent->transaction());
            return true;
        }
    }

    return QObject::event(event);
}

CObject::CObject(QObject *parent) : QObject(parent)
{
    setObjectName(QStringLiteral("CObject"));
}