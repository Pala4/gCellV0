#include "cconio.h"

#include <QTextStream>
#include <QtCore/QReadLocker>
#include <QtCore/QWriteLocker>

#include "transaction/ctransaction.h"

/*!
 * \class CStdInWorker
 */
void CStdInWorker::ioLoop()
{
    QTextStream cin(stdin);
    QString query;
    while ((QString(query).simplified().toLower() != QStringLiteral("halt")) && !isStopWork()) {
        query = cin.readLine();
        emit sendQuery(query);
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
        connect(m_stdInWorker, SIGNAL(sendQuery(QString)),
                this, SIGNAL(sendQuery(QString)));
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
void CConIO::processQuery(CTransaction *transaction)
{
    if (transaction == nullptr)
        return;

    switch (transaction->cmdID()) {
        case CConIO::GetInfo:
            transaction->sendRespons(tr("Console IO system v0.0.1"));
        break;
        case CConIO::Halt:
            stop();
            transaction->sendRespons(tr("Power down"));
        break;
        default:
        break;
    }
}

CConIO::CConIO(QObject *parent) : CObject(parent)
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
    setRespons(tr("Halted"));
}

void CConIO::setQuery(const QString &query)
{
    setRespons("Cmd# " + query);
    emit sendQuery(query);
}

void CConIO::setRespons(const QString &respons)
{
    QTextStream cout(stdout);

    cout << respons << endl;
}

void CConIO::start(void)
{   
    if (m_stdInThread == nullptr) {
        m_stdInThread = new CStdInTread(this);
        connect(m_stdInThread, SIGNAL(sendQuery(QString)),
                this, SIGNAL(sendQuery(QString)));
        connect(m_stdInThread, SIGNAL(finished()), this, SLOT(onStdInThreadStopped()));
    }

    if (!m_stdInThread->isRunning())
        m_stdInThread->start(QThread::IdlePriority);

    setRespons(tr("Client console i/o subsystem v0.0.1 is initialized"));
}

void CConIO::stop()
{
    if (m_stdInThread != nullptr)
        m_stdInThread->stopWork();
}
