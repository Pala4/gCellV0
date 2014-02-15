#ifndef CTCPSOCKET_H
#define CTCPSOCKET_H

#include "gcellcorelib_global.h"

#include <QtNetwork/QTcpSocket>

#include <QThread>

class CTcpSocket : public QTcpSocket
{
    Q_OBJECT
private:
    quint16 m_blockSize;
public:
    explicit CTcpSocket(QObject *parent = 0);
    virtual ~CTcpSocket(void);
private slots:
    void onReadyRead(void);
public slots:
    void startConnection(const QString &address, const quint16 &port);
    void startConnection(qintptr socketDescriptor);
    void stopConnection();
    void sendMessage();
};

class CSocThread : public QThread
{
    Q_OBJECT
public:
    explicit CSocThread(QObject *parent = 0) : QThread(parent){}
    virtual ~CSocThread();
};

class GCELLCORELIBSHARED_EXPORT CThreadedSocket : public QObject
{
	Q_OBJECT
private:
    quint16 m_blockSize;

    CTcpSocket *m_tcpSocket;
    CSocThread *m_socketThread;

    void initSocket();
    void killSocket();
    void killThread();
public:
    explicit CThreadedSocket(QObject *parent = 0);
    virtual ~CThreadedSocket();

    static void splitAddressPort(const QString &addressPort, QString &address, quint16 &port);

    QString hostAddress(void) const;
    quint16 hostPort(void) const;
    QAbstractSocket::SocketState state(void) const;
private slots:
    void onSocketConnected(void);
    void onSocketDisconnected(void);
    void onSocketError(const QAbstractSocket::SocketError &socketError);

    void onSocketDestroyed();
    void onThreadDestroyed();
public slots:
    void connectToHost(const QString &addressPort);
    void setSocketDescriptor(const qintptr &socketDescriptor);
    void disconnectFromHost();
signals:
    void connected(CThreadedSocket *thisSocket);
    void disconnected(CThreadedSocket *thisSocket);
    void error(CThreadedSocket *thisSocket, QString errorString,
               QAbstractSocket::SocketError error);

    void doStartConnection(QString address, quint16 port);
    void doStartConnection(qintptr socketDescriptor);
    void doStopConnection(void);
};

#endif // CTCPSOCKET_H