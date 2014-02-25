#include "cconio.h"

#include <QTextStream>
#include <QtCore/QReadLocker>
#include <QtCore/QWriteLocker>

#include "command/ccommand.h"

using namespace gcconclient;

/*!
 * \class CStdInWorker
 */
bool CStdInWorker::processInternalCommands(const QString &cmdString)
{
   if (cmdString.simplified().toLower() == QStringLiteral("halt")) {
       m_stopWork = true;
       return true;
   }

   return false;
}

void CStdInWorker::ioLoop()
{
    QTextStream cin(stdin);
    QString cmdString;
    while (!m_stopWork) {
        cmdString = cin.readLine();

        if (!processInternalCommands(cmdString))
            emit sendCmdString(cmdString);
    }
}

/*!
 * \class CStdInTread
 */
void CStdInTread::run()
{
    if (m_stdInWorker == nullptr) {
        m_stdInWorker = new CStdInWorker();
        connect(m_stdInWorker, SIGNAL(sendCmdString(QString)),
                this, SIGNAL(sendCmdString(QString)));
    }

    m_stdInWorker->ioLoop();

    m_stdInWorker->deleteLater();
    m_stdInWorker = nullptr;
}

CStdInTread::~CStdInTread()
{
    if (m_stdInWorker != nullptr)
        delete m_stdInWorker;
}

/*!
 * \class CClientOut
 */
CConIO::CConIO(QObject *parent) : QObject(parent)
{
    setObjectName(QStringLiteral("CConIO"));

    m_stdInThread = nullptr;
    m_consoleInfo = tr("Console");
}

CConIO::~CConIO(void)
{
    if ((m_stdInThread != nullptr) && m_stdInThread->isRunning())
        m_stdInThread->terminate();
}

void CConIO::onStdInThreadFinished()
{
    if (m_stdInThread != nullptr) {
        m_stdInThread->deleteLater();
        m_stdInThread = nullptr;
    }

    emit halted();
}

void CConIO::setCmdEcho(const QString &cmdString)
{
    QString cmdEchoPrefix = QStringLiteral("cmd_>");
    setMessage(cmdEchoPrefix + " " + cmdString);
}

void CConIO::setMessage(const QString &msg)
{
    QString msgPrefix = msg.startsWith(QStringLiteral("cmd_>")) ? QString()
                                                                : QStringLiteral("msg_> ");
    QTextStream cout(stdout);
    cout << msgPrefix << msg << endl;
}

void CConIO::setCmdString(const QString &cmdString, const bool &echo)
{
    if (echo)
        setCmdEcho(cmdString);

    emit sendCmdString(cmdString);
}

void CConIO::start(void)
{
    if (m_stdInThread == nullptr) {
        m_stdInThread = new CStdInTread(this);
        connect(m_stdInThread, SIGNAL(sendCmdString(QString)),
                this, SIGNAL(sendCmdString(QString)), Qt::DirectConnection);
        connect(m_stdInThread, SIGNAL(finished()), this, SLOT(onStdInThreadFinished()),
                Qt::DirectConnection);
    }

    if (!m_stdInThread->isRunning())
        m_stdInThread->start(QThread::IdlePriority);
}

bool CConIO::cmdGetConInfo(gccore::CCommand *cmd)
{
    if (cmd != nullptr)
        cmd->setRespons(consoleInfo());

    return true;
}