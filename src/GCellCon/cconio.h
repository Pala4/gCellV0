#ifndef CCONIO_H
#define CCONIO_H

#include <QThread>

#include "cobject.h"

#include <QtCore/QReadWriteLock>

class CTransactionEvent;

class CStdInWorker : public QObject
{
    Q_OBJECT
private:
    bool m_stopWork;

    mutable QReadWriteLock m_lock;
public:
    explicit CStdInWorker(QObject *parent = 0) : QObject(parent){m_stopWork = false;}

    const bool& isStopWork() const;

    void ioLoop();
public slots:
    void stopWork();
signals:
    void sendQuery(QString query);
};

class CStdInTread : public QThread
{
    Q_OBJECT
private:
    CStdInWorker *m_stdInWorker;
protected:
    virtual void run();
public:
    explicit CStdInTread(QObject *parent = 0):QThread(parent), m_stdInWorker(0){}
    virtual ~CStdInTread();
public slots:
    void stop();
signals:
    void sendQuery(QString query);
    void stopWork();
};

class CConIO : public CObject
{
    Q_OBJECT
public:
    enum ConIOQuery{GetInfo, Halt};
private:
    CStdInTread *m_stdInThread;
protected:
    void processTransactionQuery(CTransaction *transaction);
    void processTransactionRespons(CTransaction *transaction);
    bool processBackwardQuery(const QString &backwardQuery);
    bool processBackwardRespons(const QString &backwardRespons);
public:
    explicit CConIO(QObject *parent = 0);
    virtual ~CConIO();
private slots:
    void onStdInThreadStopped();
public slots:
    void start();
    void stop();
};

#endif // CCONIO_H
