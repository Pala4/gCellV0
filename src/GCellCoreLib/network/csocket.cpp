#include "csocket.h"

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QDataStream>
#include <QStringList>
#include <QWriteLocker>
#include <QReadLocker>

/*!
 * \class CSocket
 */
bool CSocket::processForwardQuery(const QString &forwardQuery)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << (quint16)0;
    out << (int)0;//query type
    out << forwardQuery;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    if (m_tcpSocket != nullptr)
        m_tcpSocket->write(block);

    return true;
}

CSocket::CSocket(QObject *parent) : CObject(parent)
{
    setObjectName(QStringLiteral("CSocket"));

    m_tcpSocket = new QTcpSocket(this);
    m_tcpSocket->setObjectName(QStringLiteral("tcpSocket"));
    connect(m_tcpSocket, SIGNAL(connected()),
            this, SLOT(onSocketConnected()), Qt::DirectConnection);
    connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(onSocketError(int)), Qt::DirectConnection);
    connect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
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
    QWriteLocker locker(&m_lock);
    if (m_tcpSocket != nullptr)
        return m_tcpSocket->peerAddress().toString();
    locker.unlock();

    return QString();
}

quint16 CSocket::hostPort() const
{
    QWriteLocker locker(&m_lock);
    if (m_tcpSocket != nullptr)
        return m_tcpSocket->peerPort();
    locker.unlock();

    return 0;
}

int CSocket::state() const
{
    QWriteLocker locker(&m_lock);
    if (m_tcpSocket != nullptr)
        return static_cast<int>(m_tcpSocket->state());
    locker.unlock();

    return static_cast<int>(QAbstractSocket::UnconnectedState);
}

void CSocket::onSocketConnected()
{
    receiveBackwardRespons(tr("Connection established to host [%1:%2]").arg(hostAddress()).
                           arg(hostPort()));
    emit connected(this);
}

void CSocket::onSocketDisconnected()
{
    receiveBackwardRespons(tr("Disconnection from host [%1:%2]").arg(hostAddress()).
                           arg(hostPort()));
    emit disconnected(this);
}

void CSocket::onSocketError(const int &socketError)
{
    if (m_tcpSocket != nullptr)
        emit error(this, m_tcpSocket->errorString(), static_cast<int>(socketError));
}

void CSocket::connectToHost(const QString &addressPort)
{
    disconnectFromHost();

    QString address;
    quint16 port = 0;
    splitAddressPort(addressPort, address, port);

    QReadLocker locker(&m_lock);
    if (m_tcpSocket != nullptr)
        m_tcpSocket->connectToHost(address, port);
    locker.unlock();
}

void CSocket::setSocketDescriptor(const qintptr &socketDescriptor)
{
    disconnectFromHost();

    QReadLocker locker(&m_lock);
    if (m_tcpSocket != nullptr)
        m_tcpSocket->setSocketDescriptor(socketDescriptor);
    locker.unlock();
}

void CSocket::disconnectFromHost()
{
    QReadLocker locker(&m_lock);
    if (m_tcpSocket != nullptr)
        m_tcpSocket->disconnectFromHost();
    locker.unlock();
}