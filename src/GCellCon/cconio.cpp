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
void CConIO::processTransactionQuery(CTransaction *transaction)
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

void CConIO::processTransactionRespons(CTransaction *transaction)
{
    if (transaction == nullptr)
        return;

    receiveBackwardRespons(transaction->respons());
}

bool CConIO::processBackwardQuery(const QString &backwardQuery)
{
    receiveBackwardRespons("Cmd# " + backwardQuery);
    receiveForwardQuery(backwardQuery);
    return true;
}

bool CConIO::processBackwardRespons(const QString &backwardRespons)
{
    QTextStream cout(stdout);
    cout << backwardRespons << endl;

    return true;
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
    receiveBackwardRespons(tr("Halted"));
}

void CConIO::start(void)
{   
    if (m_stdInThread == nullptr) {
        m_stdInThread = new CStdInTread(this);
        connect(m_stdInThread, SIGNAL(sendQuery(QString)),
                this, SLOT(receiveForwardQuery(QString)));
        connect(m_stdInThread, SIGNAL(finished()), this, SLOT(onStdInThreadStopped()));
    }

    if (!m_stdInThread->isRunning())
        m_stdInThread->start(QThread::IdlePriority);

    receiveBackwardRespons(tr("Client console i/o subsystem v0.0.1 is initialized"));
}

void CConIO::stop()
{
    if (m_stdInThread != nullptr)
        m_stdInThread->stopWork();
}
