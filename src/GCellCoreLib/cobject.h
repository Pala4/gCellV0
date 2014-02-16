#ifndef COBJECT_H
#define COBJECT_H

#include "gcellcorelib_global.h"

#include "transaction/ctransactionprocessor.h"

class CTransactionProcessor;
class CTransaction;

class GCELLCORELIBSHARED_EXPORT CObject
{
private:
    CTransactionProcessor *m_transactionProcessor;
protected:
    friend class CTransactionProcessor;
    void initTransactionProcessor();
    virtual void processTransaction(CTransaction *tranzaction);
public:
    CObject();
    virtual ~CObject();

    QObject* toQObject(){return dynamic_cast<QObject*>(this);}
};

#endif // COBJECT_H
