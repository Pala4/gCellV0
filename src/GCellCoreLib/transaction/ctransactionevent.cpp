#include "ctransactionevent.h"

CTransactionEvent::CTransactionEvent(CTransaction *transaction) :
    QEvent((QEvent::Type)CTransactionEvent::TransactionEvent)
{
    m_transaction = nullptr;

    setTransaction(transaction);
}

void CTransactionEvent::setTransaction(CTransaction *transaction)
{
    m_transaction = transaction;
}