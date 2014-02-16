#ifndef COBJECT_H
#define COBJECT_H

#include "gcellcorelib_global.h"

#include <QObject>

class CTransaction;

class GCELLCORELIBSHARED_EXPORT CObject : public QObject
{
    Q_OBJECT
protected:
    friend class CTransactionProcessor;
    virtual void processTransaction(CTransaction *tranzaction);
    bool event(QEvent *event);
public:
    explicit CObject(QObject *parent = 0);
};

#endif // COBJECT_H
