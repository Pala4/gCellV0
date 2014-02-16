#ifndef CSOCKETMNG_H
#define CSOCKETMNG_H

#include "gcellcorelib_global.h"

#include <QObject>

#include <QMap>
#include <QtNetwork/QTcpSocket>

class CSocket;

class GCELLCORELIBSHARED_EXPORT CSocketMng : public QObject
{
	Q_OBJECT
private:
    QMap<QString, CSocket*> m_sockets;

    bool addSocket(CSocket *socket, const QString &addressPort);
    void removeSocket(CSocket *socket);
public:
	explicit CSocketMng(QObject *parent = 0);
private slots:
    void onSocketConnected(CSocket *socket);
    void onSocketDisconnected(CSocket *socket);
    void onSocketError(CSocket *socket, const QString &errorString,
                       const QAbstractSocket::SocketError &error);
public slots:
    void sendMessage(const QString &address, const QString &msg);
    void sendCommand(const QString &address, const QString &cmd);
    void connectToHost(const QString &addressPort);
    void addConnection(const qintptr &socketDescriptor);
    void disconnectFromHost(const QString &addressPort);
signals:
    void newConnectionReady(CSocket *socket);
    void connectionClose(CSocket *socket);

    void messageReady(/*stCellMessage cellMsg*/);
};

#endif // CSOCKETMNG_H