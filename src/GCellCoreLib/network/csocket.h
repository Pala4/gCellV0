#ifndef CTCPSOCKET_H
#define CTCPSOCKET_H

#include "gcellcorelib_global.h"

#include "cobject.h"

#include <QtNetwork/QTcpSocket>
#include <QReadWriteLock>

class QTcpSocket;

class GCELLCORELIBSHARED_EXPORT CSocket : public CObject
{
	Q_OBJECT
public:
    enum SocketQuery{Connect, Disconnect};
private:
    mutable QReadWriteLock m_lock;
    quint16 m_blockSize;
    QTcpSocket *m_tcpSocket;
protected:
    void processTransactionQuery(CTransaction *transaction);
    bool processForwardQuery(const QString &forwardQuery);
public:
    explicit CSocket(QObject *parent = 0);

    static void splitAddressPort(const QString &addressPort, QString &address, quint16 &port);

    QString hostAddress() const;
    quint16 hostPort() const;
    QString hostName() const;
    int state() const;
private slots:
    void onSocketConnected();
    void onSocketError(const QAbstractSocket::SocketError &socketError);
    void onSocketStateChanged(const QAbstractSocket::SocketState &state);
    void onSocketDisconnected();
public slots:
    void connectToHost(const QString &addressPort);
    void setSocketDescriptor(const qintptr &socketDescriptor);
    void disconnectFromHost();
signals:
    void connected(CSocket *thisSocket);
    void disconnected(CSocket *thisSocket);
    void error(CSocket *thisSocket, QString errorString, QAbstractSocket::SocketError error);
};
//Q_DECLARE_METATYPE(QAbstractSocket::SocketState)

#endif // CTCPSOCKET_H