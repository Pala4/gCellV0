#include "csocketmng.h"

#include <QtNetwork/QHostAddress>

#include "csocket.h"

bool CSocketMng::addSocket(CThreadedSocket *socket, const QString &addressPort)
{
    if (socket == nullptr)
        return false;
    if (m_sockets.contains(addressPort)) {
        removeSocket(socket);
        return false;
    }

    m_sockets[addressPort] = socket;
    emit newConnectionReady(socket);
    return true;
}

void CSocketMng::removeSocket(CThreadedSocket *socket)
{
    if (socket == nullptr)
        return;

    if (m_sockets.values().contains(socket)) {
        QString hap = m_sockets.key(socket);
        m_sockets.remove(hap);
    }

    socket->deleteLater();
    emit connectionClose(socket);
}

CSocketMng::CSocketMng(QObject *parent) : QObject(parent)
{
    setObjectName(QStringLiteral("CSocketMng"));

    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
}

void CSocketMng::onSocketConnected(CThreadedSocket *socket)
{
    if (socket == nullptr)
        return;

    QString addressPort = socket->hostAddress() + ":" + QString("%1").arg(socket->hostPort());
    if (addSocket(socket, addressPort)) {
        //        sendMsg(tr("Connection established [address: %1; port: %2]").
        //                arg(socket->hostAddress()).arg(socket->hostPort()));
    }
}

void CSocketMng::onSocketDisconnected(CThreadedSocket *socket)
{
    //    sendMsg(tr("Disconnected"));
    removeSocket(socket);
}

void CSocketMng::onSocketError(CThreadedSocket *socket, const QString &errorString,
                               const QAbstractSocket::SocketError &error)
{
    Q_UNUSED(socket)
    Q_UNUSED(error)
    //    sendMsg(errorString);
}

void CSocketMng::sendMessage(const QString &address, const QString &msg)
{
    if (!m_sockets.contains(address))
        return;
    if (m_sockets[address] == nullptr)
        return;

    //    stCellMessage cellMsg(stCellMessage::Msg, msg);
    //    m_sockets[address]->sendMessage(cellMsg);
}

void CSocketMng::sendCommand(const QString &address, const QString &cmd)
{
    if (!m_sockets.contains(address))
        return;
    if (m_sockets[address] == nullptr)
        return;

    //    stCellMessage cellMsg(stCellMessage::Cmd, cmd);
    //    m_sockets[address]->sendMessage(cellMsg);
}

void CSocketMng::connectToHost(const QString &addressPort)
{
    if (m_sockets.contains(addressPort)) {
        //        sendMsg(tr("Connection [%1] already established").arg(addressPort));
        return;
    }

    CThreadedSocket *socket = new CThreadedSocket(this);
    connect(socket, SIGNAL(connected(CThreadedSocket*)),
            this, SLOT(onSocketConnected(CThreadedSocket*)));
    connect(socket, SIGNAL(disconnected(CThreadedSocket*)),
            this, SLOT(onSocketDisconnected(CThreadedSocket*)));
    connect(socket, SIGNAL(error(CThreadedSocket*,QString,QAbstractSocket::SocketError)),
            this, SLOT(onSocketError(CThreadedSocket*,QString,QAbstractSocket::SocketError)));

    socket->connectToHost(addressPort);
}

void CSocketMng::addConnection(const qintptr &socketDescriptor)
{
    CThreadedSocket *socket = new CThreadedSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, SIGNAL(disconnected(CThreadedSocket*)),
            this, SLOT(onSocketDisconnected(CThreadedSocket*)));
    connect(socket, SIGNAL(error(CThreadedSocket*,QString,QAbstractSocket::SocketError)),
            this, SLOT(onSocketError(CThreadedSocket*,QString,QAbstractSocket::SocketError)));

    QString addressPort = socket->hostAddress() + ":" + QString("%1").arg(socket->hostPort());
    if (!addSocket(socket, addressPort)) {
    }
}

void CSocketMng::disconnectFromHost(const QString &addressPort)
{
    if (!m_sockets.contains(addressPort)) {
        QString address;
        quint16 port = 0;
        CThreadedSocket::splitAddressPort(addressPort, address, port);
        //        sendMsg(tr("Connection [IP: %1; Port: %2] not established").arg(address).arg(port));
        return;
    }

    if (m_sockets[addressPort] != nullptr)
        m_sockets[addressPort]->disconnectFromHost();
}