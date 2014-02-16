#ifndef CCONIO_H
#define CCONIO_H

#include <QObject>
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

class CConIO : public QObject, public CObject
{
    Q_OBJECT
public:
    enum ConIOQuery{GetInfo, Halt};
private:
    CStdInTread *m_stdInThread;
protected:
    void processTransaction(CTransaction *transaction);
public:
    explicit CConIO(QObject *parent = 0);
    virtual ~CConIO();
private slots:
    void onStdInThreadStopped();
public slots:
    void setQuery(const QString &query);
    void setRespons(const QString &respons);
    void start();
    void stop();
signals:
    void sendQuery(QString query);
    void sendRespons(QString respons);
};

#endif // CCONIO_H
