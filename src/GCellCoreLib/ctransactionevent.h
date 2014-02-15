#ifndef CTRANSACTIONEVENT_H
#define CTRANSACTIONEVENT_H

#include "gcellcorelib_global.h"

#include <QEvent>

class CTransaction;

class GCELLCORELIBSHARED_EXPORT CTransactionEvent : public QEvent
{
public:
    enum EventType{TransactionEvent = QEvent::User + 1};
private:
    CTransaction *m_transaction;
public:
    explicit CTransactionEvent(CTransaction *transaction);

    CTransaction* transaction() const{return m_transaction;}
    void setTransaction(CTransaction *transaction);
};

#endif // CTRANSACTIONEVENT_H
