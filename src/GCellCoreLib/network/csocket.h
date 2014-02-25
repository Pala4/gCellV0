#ifndef CTCPSOCKET_H
#define CTCPSOCKET_H

#include "gcellcorelib_global.h"

#include <QObject>

#include <QtNetwork/QTcpSocket>
#include <QReadWriteLock>

class QTcpSocket;

namespace gccore {

class CCommand;

class GCELLCORELIBSHARED_EXPORT CSocket : public QObject
{
	Q_OBJECT
private:
    mutable QReadWriteLock m_lock;
    quint16 m_blockSize;
    QTcpSocket *m_tcpSocket;    
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
    void onSocketReadyRead();

    void setMessage(const QString &msg);
public slots:
    void connectToHost(const QString &addressPort);
    void setSocketDescriptor(const qintptr &socketDescriptor);
    void disconnectFromHost();
    void netSendData(const QString &data, const int &dataType);

    bool cmdConnectToHost(CCommand *cmd, const QString &addressPort);
    bool cmdDisconnectFromHost(CCommand *cmd);
signals:
    void connected(CSocket *thisSocket);
    void disconnected(CSocket *thisSocket);
    void error(CSocket *thisSocket, QString errorString, QAbstractSocket::SocketError error);
    void netReceiveData(QString data, int dataType);

    void sendMessage(QString msg);
};
//Q_DECLARE_METATYPE(QAbstractSocket::SocketState)

}

#endif // CTCPSOCKET_H