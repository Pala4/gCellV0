#include "csocketmng.h"

#include <QtNetwork/QHostAddress>

#include "csocket.h"

using namespace gccore;

bool CSocketMng::addSocket(CSocket *socket, const QString &addressPort)
{
    if ((socket == nullptr) || addressPort.isEmpty())
        return false;
    if (m_sockets.contains(addressPort))
        return false;

    m_sockets[addressPort] = socket;
    return true;
}

CSocketMng::CSocketMng(QObject *parent) : QObject(parent)
{
    setObjectName(QStringLiteral("CSocketMng"));

    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
}

void CSocketMng::onSocketDisconnected(CSocket *socket)
{
    if (socket == nullptr)
        return;
    if (!m_sockets.values().contains(socket))
        return;

    m_sockets.remove(m_sockets.key(socket));
    socket->deleteLater();
}

void CSocketMng::onNetReceiveData(const QString &data, const int &dataType)
{
    CSocket *socket = qobject_cast<CSocket*>(sender());
    if (socket == nullptr)
        return;
    if (!m_sockets.values().contains(socket))
        return;

    QString addressPort = m_sockets.key(socket, QString());
    if (addressPort.isEmpty())
        return;

    emit netReceiveData(addressPort, data, dataType);
}

void CSocketMng::addConnection(const qintptr &socketDescriptor)
{
    CSocket *socket = new CSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, SIGNAL(disconnected(CSocket*)),
            this, SLOT(onSocketDisconnected(CSocket*)));
    connect(socket, SIGNAL(netReceiveData(QString,int)), this, SLOT(onNetReceiveData(QString,int)));
    QString addressPort = socket->hostAddress() + ":" + QString("%1").arg(socket->hostPort());
    if (!addSocket(socket, addressPort))
        socket->deleteLater();
}

void CSocketMng::disconnectFromHost(const QString &addressPort)
{
    if (!m_sockets.contains(addressPort))
        return;

    if (m_sockets[addressPort] != nullptr)
        m_sockets[addressPort]->disconnectFromHost();
}

void CSocketMng::netSendData(const QString &addressPort, const QString &data, const int dataType)
{
    if (!m_sockets.contains(addressPort))
        return;
    if (m_sockets[addressPort] != nullptr)
        m_sockets[addressPort]->netSendData(data, dataType);
}