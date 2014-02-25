#ifndef CCONIO_H
#define CCONIO_H

#include <QThread>

#include <QtCore/QReadWriteLock>

namespace gccore {
class CCommand;
}

namespace gcconclient {

class CStdInWorker : public QObject
{
    Q_OBJECT
private:
    bool m_stopWork;
    mutable QReadWriteLock m_lock;

    bool processInternalCommands(const QString &cmdString);
public:
    explicit CStdInWorker(QObject *parent = 0) : QObject(parent){m_stopWork = false;}

    void ioLoop();
signals:
    void sendCmdString(QString cmdString);
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
signals:
    void sendCmdString(QString cmdString);
};

class CConIO : public QObject
{
    Q_OBJECT
private:
    CStdInTread *m_stdInThread;
    QString m_consoleInfo;
public:
    explicit CConIO(QObject *parent = 0);
    virtual ~CConIO();

    const QString& consoleInfo() const{return m_consoleInfo;}
    void setConsoleInfo(const QString &consoleInfo){m_consoleInfo = consoleInfo;}
private slots:
    void onStdInThreadFinished();

    void setCmdEcho(const QString &cmdString);
public slots:
    void setMessage(const QString &msg);
    void setCmdString(const QString &cmdString, const bool &echo = true);
    void start();

    bool cmdGetConInfo(gccore::CCommand *cmd);
signals:
    void sendCmdString(QString cmdString);
    void halted();
};

}

#endif // CCONIO_H