#include "cserver.h"

using namespace gccore;

/*!
 * \class CServer
 */
void CTcpServer::incomingConnection(qintptr socketDescriptor)
{
    emit hasIncomingConnection(socketDescriptor);
}

CTcpServer::CTcpServer(QObject *parent) : QTcpServer(parent)
{
    setObjectName(QStringLiteral("CTcpServer"));
}

/*!
 * \class CTcpServer
 */
CServer::CServer(QObject *parent) : QObject(parent)
{
    setObjectName(QStringLiteral("CTcpServer"));

    m_tcpServer = new CTcpServer(this);
    m_tcpServer->setObjectName(QStringLiteral("tcpServer"));
    connect(m_tcpServer, SIGNAL(hasIncomingConnection(qintptr)),
            this, SIGNAL(newConnection(qintptr)));
    connect(m_tcpServer, SIGNAL(destroyed()), this, SLOT(onTcpServerDestroyed()));
}

QString CServer::start(const QString &address, const quint16 &port)
{
    Q_ASSERT(m_tcpServer != nullptr);

    QHostAddress ha;
    if (!address.isEmpty())
        ha.setAddress(address);

    if (!m_tcpServer->listen(ha, port)) {
        m_tcpServer->close();
        return tr("Unable to start server [%1]").arg(m_tcpServer->errorString());
    }

    return tr("Server started at [%1:%2]").arg(address).arg(port);
}

QString CServer::stop()
{
    Q_ASSERT(m_tcpServer != nullptr);

    m_tcpServer->close();
    return tr("Server stopped");
}

void CServer::onTcpServerDestroyed()
{
    m_tcpServer = nullptr;
}