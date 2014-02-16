#ifndef CTRANSACTIONPROCESSOR_H
#define CTRANSACTIONPROCESSOR_H

#include "gcellcorelib_global.h"

#include <QObject>

#include "cobject.h"

class GCELLCORELIBSHARED_EXPORT CTransactionProcessor : public QObject
{
    Q_OBJECT
private:
    friend class CObject;
    void setObject(QObject *object);
protected:
    bool eventFilter(QObject *object, QEvent *event);
public:
    explicit CTransactionProcessor(QObject *parent = 0);
};

#endif // CTRANSACTIONPROCESSOR_H
