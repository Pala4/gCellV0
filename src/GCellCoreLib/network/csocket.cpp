#include "csocket.h"

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QDataStream>
#include <QStringList>
#include <QWriteLocker>
#include <QReadLocker>

using namespace gccore;

/*!
 * \class CSocket
 */
CSocket::CSocket(QObject *parent) : QObject(parent)
{
    setObjectName(QStringLiteral("CSocket"));

    m_blockSize = 0;

    m_tcpSocket = new QTcpSocket(this);
    m_tcpSocket->setObjectName(QStringLiteral("tcpSocket"));
    connect(m_tcpSocket, SIGNAL(connected()),
            this, SLOT(onSocketConnected()), Qt::DirectConnection);
    connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(onSocketError(QAbstractSocket::SocketError)), Qt::DirectConnection);
    connect(m_tcpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(onSocketStateChanged(QAbstractSocket::SocketState)), Qt::DirectConnection);
    connect(m_tcpSocket, SIGNAL(disconnected()),
            this, SLOT(onSocketDisconnected()), Qt::DirectConnection);
    connect(m_tcpSocket, SIGNAL(readyRead()),
            this, SLOT(onSocketReadyRead()), Qt::DirectConnection);

    qRegisterMetaType<QAbstractSocket::SocketState>("SocketState");
}

void CSocket::splitAddressPort(const QString &addressPort, QString &address, quint16 &port)
{
    QStringList hapList = addressPort.split(QChar(':'));
    address = hapList.count() > 0 ? hapList.at(0) : QString();
    if (hapList.count() > 1) {
        bool ok = false;
        quint16 _port = hapList.at(1).toInt(&ok);
        port = ok ? _port : 0;
    }
}

QString CSocket::hostAddress() const
{
    if (m_tcpSocket != nullptr) {
        QWriteLocker locker(&m_lock);
        return m_tcpSocket->peerAddress().toString();
    }

    return QString();
}

quint16 CSocket::hostPort() const
{
    if (m_tcpSocket != nullptr) {
        QWriteLocker locker(&m_lock);
        return m_tcpSocket->peerPort();
    }

    return 0;
}

QString CSocket::hostName() const
{
    if (m_tcpSocket != nullptr) {
        QWriteLocker locker(&m_lock);
        return m_tcpSocket->peerName();
    }

    return QString();
}

int CSocket::state() const
{
    if (m_tcpSocket != nullptr) {
        QWriteLocker locker(&m_lock);
        return static_cast<int>(m_tcpSocket->state());
    }

    return static_cast<int>(QAbstractSocket::UnconnectedState);
}

void CSocket::onSocketConnected()
{
    QString hostAddress;
    quint16 hostPort = 0;
    QString hostName;
    if (m_tcpSocket != nullptr) {
        hostAddress = m_tcpSocket->peerAddress().toString();
        hostPort = m_tcpSocket->peerPort();
        hostName = m_tcpSocket->peerName();
    }

    setMessage(tr("Connection established to host [%1:%2 (%3)]").arg(hostAddress).
               arg(hostPort).arg(hostName));
    emit connected(this);
}

void CSocket::onSocketError(const QAbstractSocket::SocketError &socketError)
{
    if (m_tcpSocket != nullptr)
        emit error(this, m_tcpSocket->errorString(), socketError);
}

void CSocket::onSocketStateChanged(const QAbstractSocket::SocketState &state)
{
    switch (state) {
        case QAbstractSocket::UnconnectedState:
            setMessage(tr("Unconnected"));
        break;
        case QAbstractSocket::HostLookupState:
            setMessage(tr("Performed a host name lookup..."));
        break;
        case QAbstractSocket::ConnectingState:
            setMessage(tr("Begins to establish a connection..."));
        break;
        case QAbstractSocket::ConnectedState:
            setMessage(tr("Connected"));
        break;
        case QAbstractSocket::BoundState:
            setMessage(tr("Bound state"));
        break;
        case QAbstractSocket::ClosingState:
            setMessage(tr("Connection closing..."));
        break;
        default:
        break;
    }
}

void CSocket::onSocketDisconnected()
{   
    QString hostAddress;
    quint16 hostPort = 0;
    QString hostName;
    if (m_tcpSocket != nullptr) {
        hostAddress = m_tcpSocket->peerAddress().toString();
        hostPort = m_tcpSocket->peerPort();
        hostName = m_tcpSocket->peerName();
    }

    setMessage(tr("Disconnected from host [%1:%2 (%3)]").arg(hostAddress).
               arg(hostPort).arg(hostName));
    emit disconnected(this);
}

void CSocket::onSocketReadyRead()
{
    if (m_tcpSocket == nullptr)
        return;

    QDataStream in(m_tcpSocket);
    in.setVersion(QDataStream::Qt_5_2);

    if (m_blockSize == 0) {
        if (m_tcpSocket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> m_blockSize;
    }
    if (m_tcpSocket->bytesAvailable() < m_blockSize)
        return;
    m_blockSize = 0;

    int dataType;
    QString data;
    in >> dataType;
    in >> data;

    emit netReceiveData(data, dataType);
}

void CSocket::setMessage(const QString &msg)
{
    emit sendMessage(msg);
}

void CSocket::connectToHost(const QString &addressPort)
{
    disconnectFromHost();

    QString address;
    quint16 port = 0;
    splitAddressPort(addressPort, address, port);

    if (m_tcpSocket != nullptr) {
        QReadLocker locker(&m_lock);
        m_tcpSocket->connectToHost(address, port);
        locker.unlock();
    }
}

void CSocket::setSocketDescriptor(const qintptr &socketDescriptor)
{
    disconnectFromHost();

    if (m_tcpSocket != nullptr) {
        QReadLocker locker(&m_lock);
        m_tcpSocket->setSocketDescriptor(socketDescriptor);
        locker.unlock();
    }
}

void CSocket::disconnectFromHost()
{
    if (m_tcpSocket != nullptr) {
        QReadLocker locker(&m_lock);
        m_tcpSocket->disconnectFromHost();
        locker.unlock();
    }
}

void CSocket::netSendData(const QString &data, const int &dataType)
{
    if (m_tcpSocket == nullptr)
        return;
    if (m_tcpSocket->state() != QAbstractSocket::ConnectedState)
        return;

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << (quint16)0;
    out << (int)dataType;
    out << data;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    m_tcpSocket->write(block);
}

bool CSocket::cmdConnectToHost(CCommand *cmd, const QString &addressPort)
{
    Q_UNUSED(cmd)
    connectToHost(addressPort);
    return true;
}

bool CSocket::cmdDisconnectFromHost(CCommand *cmd)
{
    Q_UNUSED(cmd)
    disconnectFromHost();
    return true;
}