#include "ctransactionprocessor.h"

#include "ctransactionevent.h"

void CTransactionProcessor::setObject(QObject *object)
{
    if (object != nullptr)
        object->installEventFilter(this);
}

bool CTransactionProcessor::eventFilter(QObject *object, QEvent *event)
{
    if (static_cast<int>(event->type()) == static_cast<int>(CTransactionEvent::TransactionEvent)) {
        CTransactionEvent *transactionEvent = dynamic_cast<CTransactionEvent*>(event);
        if (transactionEvent != nullptr) {
            CObject *baseObject = dynamic_cast<CObject*>(object);
            if (baseObject != nullptr) {
                baseObject->processTransaction(transactionEvent->transaction());
                return true;
            }
        }
    }

    return false;
}

CTransactionProcessor::CTransactionProcessor(QObject *parent) : QObject(parent)
{
    setObjectName(QStringLiteral("CTransactionProcessor"));
}
