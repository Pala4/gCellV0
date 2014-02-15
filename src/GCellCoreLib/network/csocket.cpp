#include "csocket.h"

#include <QtNetwork/QHostAddress>
#include <QDataStream>
#include <QStringList>

/*!
 * \class CTcpSocket
 */
CTcpSocket::CTcpSocket(QObject *parent) : QTcpSocket(parent)
{
    setObjectName(QStringLiteral("CTcpSocket"));

    m_blockSize = 0;

    connect(this, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}
#include <QDebug>
CTcpSocket::~CTcpSocket(void)
{
    unsigned long long i = 0;
    while (++i < 1000000000);
    qDebug() << "Soc destroyed";
}

void CTcpSocket::onReadyRead(void)
{
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_5_2);

    if(m_blockSize == 0)
    {
        if(bytesAvailable() < (int)sizeof(quint16)) return;
        in >> m_blockSize;
    }
    if(bytesAvailable() < m_blockSize) return;
    m_blockSize = 0;

    int msgType;
    QString msg;
    in >> msgType;
    in >> msg;
}

void CTcpSocket::startConnection(const QString &address, const quint16 &port)
{
    connectToHost(address, port);
    if (!waitForConnected())
        stopConnection();
}

void CTcpSocket::startConnection(qintptr socketDescriptor)
{
    setSocketDescriptor(socketDescriptor);
}

void CTcpSocket::stopConnection()
{
    disconnectFromHost();
    waitForDisconnected();
}

void CTcpSocket::sendMessage()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << (quint16)0;
//    out << (int)cellMsg.msgType();
//    out << cellMsg.msg();
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    write(block);
    waitForBytesWritten();
}

CSocThread::~CSocThread()
{
    qDebug() << "Soc thread destroyed";
}

/*!
 * \class CSocket
 */
void CThreadedSocket::initSocket()
{
    if ((m_tcpSocket != nullptr) || (m_socketThread != nullptr))
        disconnectFromHost();

    m_tcpSocket = new CTcpSocket();
    connect(m_tcpSocket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
    connect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
    connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(onSocketError(QAbstractSocket::SocketError)));
    connect(this, SIGNAL(doStartConnection(QString,quint16)),
            m_tcpSocket, SLOT(startConnection(QString,quint16)));
    connect(this, SIGNAL(doStartConnection(qintptr)), m_tcpSocket, SLOT(startConnection(qintptr)));
    connect(this, SIGNAL(doStopConnection()), m_tcpSocket, SLOT(stopConnection()));
    connect(m_tcpSocket, SIGNAL(destroyed()), this, SLOT(onSocketDestroyed()));

    m_socketThread = new CSocThread(this);
    connect(m_socketThread, SIGNAL(destroyed()), this, SLOT(onThreadDestroyed()));

    m_socketThread->start();
    m_tcpSocket->moveToThread(m_socketThread);
}

void CThreadedSocket::killSocket()
{
    if (m_tcpSocket != nullptr) {
        m_tcpSocket->deleteLater();
        m_tcpSocket = nullptr;
    }
}

void CThreadedSocket::killThread()
{
    if (m_socketThread != nullptr) {
        if (m_socketThread->isRunning()) {
            m_socketThread->quit();
            m_socketThread->wait();
        }

        delete m_socketThread;
        m_socketThread = nullptr;
    }
}

CThreadedSocket::CThreadedSocket(QObject *parent) : QObject(parent)
{
    setObjectName(QStringLiteral("CThreadedSocket"));

    m_tcpSocket = nullptr;
    m_socketThread = nullptr;
}

CThreadedSocket::~CThreadedSocket()
{
    killSocket();
    killThread();
    qDebug() << "Tr soc destroyed";
}

void CThreadedSocket::splitAddressPort(const QString &addressPort, QString &address, quint16 &port)
{
    QStringList hapList = addressPort.split(QChar(':'));
    address = hapList.count() > 0 ? hapList.at(0) : QString();
    if (hapList.count() > 1) {
        bool ok = false;
        quint16 _port = hapList.at(1).toInt(&ok);
        port = ok ? _port : 0;
    }
}

QString CThreadedSocket::hostAddress(void) const
{
    //Mutex
    if (m_tcpSocket != nullptr)
        return m_tcpSocket->peerAddress().toString();

    return QString();
}

quint16 CThreadedSocket::hostPort(void) const
{
    //Mutex
    if (m_tcpSocket != nullptr)
        return m_tcpSocket->peerPort();

    return 0;
}

QAbstractSocket::SocketState CThreadedSocket::state(void) const
{
    //Mutex
    if (m_tcpSocket != nullptr)
        return m_tcpSocket->state();

    return QAbstractSocket::UnconnectedState;
}

void CThreadedSocket::onSocketConnected(void)
{
    emit connected(this);
}

void CThreadedSocket::onSocketDisconnected(void)
{
    emit disconnected(this);
}

void CThreadedSocket::onSocketError(const QAbstractSocket::SocketError &socketError)
{
    //Mutex
    if (m_tcpSocket != nullptr)
        emit error(this, m_tcpSocket->errorString(), socketError);
}

void CThreadedSocket::onSocketDestroyed()
{
    killThread();
}

void CThreadedSocket::onThreadDestroyed()
{
    killSocket();
}

void CThreadedSocket::connectToHost(const QString &addressPort)
{
    initSocket();

    QString address;
    quint16 port = 0;
    splitAddressPort(addressPort, address, port);
    emit doStartConnection(address, port);
}

void CThreadedSocket::setSocketDescriptor(const qintptr &socketDescriptor)
{
    initSocket();
    emit doStartConnection(socketDescriptor);
}

void CThreadedSocket::disconnectFromHost()
{
    emit doStopConnection();
}