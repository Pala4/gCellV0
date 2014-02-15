#ifndef CBASE_H
#define CBASE_H

#include "gcellcorelib_global.h"

#include "transaction/ctransactionprocessor.h"

class CTransactionProcessor;
class CTransaction;

class GCELLCORELIBSHARED_EXPORT CBase
{
private:
    CTransactionProcessor *m_transactionProcessor;
protected:
    friend class CTransactionProcessor;
    void initTransactionProcessor();
    virtual void processTransaction(CTransaction *tranzaction);
public:
    CBase();
    virtual ~CBase();

    QObject* toQObject(){return dynamic_cast<QObject*>(this);}
};

#endif // CBASE_H
