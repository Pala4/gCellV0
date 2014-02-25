#include "cconsoleapp.h"

#include "cconio.h"
#include "command/ccommanddsp.h"
#include "network/csocket.h"

using namespace gcconclient;

CConsoleApp::CConsoleApp(int &argc, char **argv) : QCoreApplication(argc, argv)
{
    setObjectName(QStringLiteral("CConsoleApp"));
    connect(this, SIGNAL(aboutToQuit()), this, SLOT(onAboutToQuit()));

    m_commandDsp = new gccore::CCommandDsp(this);
    m_commandDsp->setObjectName(QStringLiteral("commandDsp"));

    m_conIO = new CConIO(this);
    m_conIO->setObjectName(QStringLiteral("conIO"));
    m_conIO->setConsoleInfo(tr("Console io subsystem v0.0.0"));
    connect(m_conIO, SIGNAL(sendCmdString(QString)), this, SLOT(onReceiveCmdString(QString)));
    connect(m_conIO, SIGNAL(halted()), this, SLOT(quit()));

    m_socket = new gccore::CSocket(this);
    m_socket->setObjectName(QStringLiteral("socket"));
    connect(m_socket, SIGNAL(sendMessage(QString)), this, SLOT(setMessage(QString)));
    connect(m_socket, SIGNAL(netReceiveData(QString,int)),
            this, SLOT(onNetReceiveData(QString,int)));

    m_commandDsp->registerCommand(m_conIO, SLOT(cmdGetConInfo(gccore::CCommand*)),
                                  QStringLiteral("getConInfo"));
    m_commandDsp->registerCommand(m_socket, SLOT(cmdConnectToHost(CCommand*,QString)),
                                  QStringLiteral("connect"));
    m_commandDsp->registerCommand(m_socket, SLOT(cmdDisconnectFromHost(CCommand*)),
                                  QStringLiteral("disconnect"));
    m_commandDsp->registerCommand(this, SLOT(cmdNetSendMessage(gccore::CCommand*,QString)),
                                  QStringLiteral("netSendMsg"));
    m_commandDsp->registerCommand(this, SLOT(cmdNetSendCommand(gccore::CCommand*,QString)),
                                  QStringLiteral("netSendCmd"));

    m_conIO->setCmdString(QStringLiteral("getConInfo"), false);
    m_conIO->start();
}

void CConsoleApp::onReceiveCmdString(const QString &cmdString)
{
    Q_ASSERT(m_commandDsp != nullptr);

    QString respons;
    int errcode = ERR_NOT_ERROR;
    QString errdesc = ERR_NOT_ERROR_STR;
    QString cmdResultString;
    if (!m_commandDsp->execute(QString(), cmdString, respons, errcode, errdesc))
        cmdResultString = QString("Error #%1 [%2]").arg(errcode).arg(errdesc);
    else
        cmdResultString = respons;

    if (!cmdResultString.isEmpty())
        setMessage(cmdResultString);
}

void CConsoleApp::setMessage(const QString &msg)
{
    if (m_conIO == nullptr)
        return;

    m_conIO->setMessage(msg);
}

void CConsoleApp::onAboutToQuit()
{
    if ((m_socket != nullptr) && (m_socket->state() == QAbstractSocket::ConnectedState)) {
        m_socket->disconnectFromHost();
        //wait
        //close
    }

    setMessage(tr("System halted"));
    setMessage(tr("Power down"));
}

void CConsoleApp::onNetReceiveData(const QString &data, const int &dataType)
{
    switch (dataType) {
        case 0:
            setMessage(data);
        break;
        case 1:
            onReceiveCmdString(data);
        break;
        default:
        break;
    }
}

bool CConsoleApp::cmdNetSendMessage(gccore::CCommand *cmd, const QString &msg)
{
    Q_UNUSED(cmd)
    if (m_socket == nullptr)
        return false;
    m_socket->netSendData(msg, 0);
    return true;
}

bool CConsoleApp::cmdNetSendCommand(gccore::CCommand *cmd, const QString &cmdString)
{
    Q_UNUSED(cmd)
    if (m_socket == nullptr)
        return false;
    m_socket->netSendData(cmdString, 1);
    return true;
}