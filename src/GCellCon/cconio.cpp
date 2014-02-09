#include "cconio.h"

#include <QTextStream>
#include <QtCore/QReadLocker>
#include <QtCore/QWriteLocker>

/*!
 * \class CStdInWorker
 */
void CStdInWorker::ioLoop()
{
    QTextStream cin(stdin);
    QString cmd;
    while ((cmd != QStringLiteral("halt")) && !isStopWork()) {
        cmd = cin.readLine();
        emit sendCmd(cmd);
    }
}

const bool& CStdInWorker::isStopWork() const
{
    QWriteLocker writeLock(&m_lock);
    return m_stopWork;
}

void CStdInWorker::stopWork()
{
    QReadLocker readLock(&m_lock);
    m_stopWork = true;
}

/*!
 * \class CStdInTread
 */
void CStdInTread::run()
{
    if (m_stdInWorker == nullptr) {
        m_stdInWorker = new CStdInWorker();
        connect(m_stdInWorker, SIGNAL(sendCmd(QString)),
                this, SIGNAL(sendCmd(QString)));
        connect(this, SIGNAL(stopWork()), m_stdInWorker, SLOT(stopWork()));
    }

    m_stdInWorker->ioLoop();

    m_stdInWorker->deleteLater();
    m_stdInWorker = nullptr;
}

CStdInTread::~CStdInTread()
{
    stop();
    if (m_stdInWorker != nullptr)
        delete m_stdInWorker;
}

void CStdInTread::stop()
{
    emit stopWork();
}

/*!
 * \class CClientOut
 */
CConIO::CConIO(QObject *parent) : QObject(parent)
{
    setObjectName(QStringLiteral("CConIO"));

    m_stdInThread = nullptr;
}

CConIO::~CConIO(void)
{
    if ((m_stdInThread != nullptr) && m_stdInThread->isRunning())
        m_stdInThread->terminate();
}

void CConIO::onStdInThreadStopped()
{
    outMsg(tr("Halted"));
}

void CConIO::outMsg(const QString &msg)
{
    QTextStream cout(stdout);

    cout << msg << endl;
}

void CConIO::setCmd(const QString &cmd)
{
    outMsg("Cmd# " + cmd);
    emit sendCmd(cmd);
}

void CConIO::start(void)
{   
    if (m_stdInThread == nullptr) {
        m_stdInThread = new CStdInTread(this);
        connect(m_stdInThread, SIGNAL(sendCmd(QString)),
                this, SIGNAL(sendCmd(QString)));
        connect(m_stdInThread, SIGNAL(finished()), this, SLOT(onStdInThreadStopped()));
    }

    if (!m_stdInThread->isRunning())
        m_stdInThread->start(QThread::IdlePriority);

    outMsg(tr("Client console i/o subsystem v0.0.1 is initialized"));
}

void CConIO::stop()
{
    if (m_stdInThread != nullptr)
        m_stdInThread->stopWork();
}
