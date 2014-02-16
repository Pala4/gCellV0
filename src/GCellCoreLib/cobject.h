#ifndef COBJECT_H
#define COBJECT_H

#include "gcellcorelib_global.h"

#include <QObject>

class CTransaction;

class GCELLCORELIBSHARED_EXPORT CObject : public QObject
{
    Q_OBJECT
protected:
    virtual void processTransactionQuery(CTransaction *transaction);
    virtual void processTransactionRespons(CTransaction *transaction);
    bool event(QEvent *event);
    virtual bool processBackwardQuery(const QString &backwardQuery);
    virtual bool processBackwardRespons(const QString &backwardRespons);
    virtual bool processForwardQuery(const QString &forwardQuery);
    virtual bool processForwardRespons(const QString &forwardRespons);
public:
    explicit CObject(QObject *parent = 0);
public slots:
    void receiveBackwardQuery(const QString &query);
    void receiveBackwardRespons(const QString &respons);
    void receiveForwardQuery(const QString &query);
    void receiveForwardRespons(const QString &respons);
signals:
    void transmitBackwardQuery(QString query);
    void transmitBackwardRespons(QString respons);
    void transmitForwardQuery(QString query);
    void transmitForwardRespons(QString respons);
};

#endif // COBJECT_H