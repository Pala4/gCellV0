#ifndef CSOCKETMNG_H
#define CSOCKETMNG_H

#include "gcellcorelib_global.h"

#include <QObject>

#include <QMap>
#include <QtNetwork/QTcpSocket>

class CThreadedSocket;

class GCELLCORELIBSHARED_EXPORT CSocketMng : public QObject
{
	Q_OBJECT
private:
    QMap<QString, CThreadedSocket*> m_sockets;

    bool addSocket(CThreadedSocket *socket, const QString &addressPort);
    void removeSocket(CThreadedSocket *socket);
public:
	explicit CSocketMng(QObject *parent = 0);
private slots:
    void onSocketConnected(CThreadedSocket *socket);
    void onSocketDisconnected(CThreadedSocket *socket);
    void onSocketError(CThreadedSocket *socket, const QString &errorString,
                       const QAbstractSocket::SocketError &error);
public slots:
    void sendMessage(const QString &address, const QString &msg);
    void sendCommand(const QString &address, const QString &cmd);
    void connectToHost(const QString &addressPort);
    void addConnection(const qintptr &socketDescriptor);
    void disconnectFromHost(const QString &addressPort);
signals:
    void newConnectionReady(CThreadedSocket *socket);
    void connectionClose(CThreadedSocket *socket);

    void messageReady(/*stCellMessage cellMsg*/);
};

#endif // CSOCKETMNG_H