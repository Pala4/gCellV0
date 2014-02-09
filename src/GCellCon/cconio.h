#ifndef CCONIO_H
#define CCONIO_H

#include <QObject>
#include <QThread>

#include <QtCore/QReadWriteLock>

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
    void sendCmd(QString cmd);
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
    void sendCmd(QString cmd);
    void stopWork();
};

class CConIO : public QObject
{
    Q_OBJECT
private:
    CStdInTread *m_stdInThread;
public:
    explicit CConIO(QObject *parent = 0);
    virtual ~CConIO();
private slots:
    void onStdInThreadStopped();
public slots:
    void setMsg(const QString &msg);
    void setCmd(const QString &cmd);
    void start();
    void stop();
signals:
    void sendCmd(QString cmd);
};

#endif // CCONIO_H
