#include "cserverapp.h"

#include "command/ccommanddsp.h"
#include "network/cserver.h"
#include "network/csocketmng.h"

using namespace gcserver;

CServerApp::CServerApp(int &argc, char **argv) : QCoreApplication(argc, argv)
{
    setObjectName(QStringLiteral("CServerApp"));

    m_commandDsp = new gccore::CCommandDsp(this);
    m_commandDsp->setObjectName(QStringLiteral("commandDsp"));

    m_server = new gccore::CServer(this);
    m_server->setObjectName(QStringLiteral("server"));

    m_socketMng = new gccore::CSocketMng(this);
    m_socketMng->setObjectName(QStringLiteral("socketMng"));
    connect(m_server, SIGNAL(newConnection(qintptr)), m_socketMng, SLOT(addConnection(qintptr)));
    connect(m_socketMng, SIGNAL(netReceiveData(QString,QString,int)),
            this, SLOT(onNetReceiveData(QString,QString,int)));

    m_commandDsp->registerCommand(this, SLOT(cmdDisconnect(gccore::CCommand*)),
                                  QStringLiteral("disconnect"));
    m_commandDsp->registerCommand(this, SLOT(cmdSrvInfo(gccore::CCommand*)),
                                  QStringLiteral("getSrvInfo"));
    m_commandDsp->registerCommand(this, SLOT(cmdHalt(gccore::CCommand*)), QStringLiteral("halt"));
}

void CServerApp::start()
{
    if (m_server != nullptr)
        m_server->start("127.0.0.1", 33333);
}

void CServerApp::onReceiveCmdString(const QString &senderID, const QString &cmdString)
{
    Q_ASSERT(m_commandDsp != nullptr);

    QString respons;
    int errcode = ERR_NOT_ERROR;
    QString errdesc = ERR_NOT_ERROR_STR;
    QString cmdResultString;
    if (!m_commandDsp->execute(senderID, cmdString, respons, errcode, errdesc))
        cmdResultString = QString("Error #%1 [%2]").arg(errcode).arg(errdesc);
    else
        cmdResultString = respons;

    if (!cmdResultString.isEmpty())
        setMessage(senderID, cmdResultString);
}

void CServerApp::setMessage(const QString &receiverID, const QString &msg)
{
    if (m_socketMng == nullptr)
        return;

    m_socketMng->netSendData(receiverID, msg, 0);
}

void CServerApp::onAboutToQuit()
{
}

void CServerApp::onNetReceiveData(const QString &senderID, const QString &data, const int &dataType)
{
    switch (dataType) {
        case 0:
        break;
        case 1:
            onReceiveCmdString(senderID, data);
        break;
        default:
        break;
    }
}

bool CServerApp::cmdDisconnect(gccore::CCommand *cmd)
{
    if ((m_socketMng == nullptr) || (cmd == nullptr))
        return false;

    m_socketMng->disconnectFromHost(cmd->senderID());
    cmd->setRespons(tr("Srv disconnected"));
    return true;
}

bool CServerApp::cmdSrvInfo(gccore::CCommand *cmd)
{
    if (cmd != nullptr)
        cmd->setRespons(tr("Gamma cell server v0.0.0"));

    return true;
}

bool CServerApp::cmdHalt(gccore::CCommand *cmd)
{
    Q_UNUSED(cmd)
    quit();
    return true;
}