#ifndef CTCPSOCKET_H
#define CTCPSOCKET_H

#include "gcellcorelib_global.h"

#include "cobject.h"

#include <QReadWriteLock>

class QTcpSocket;

class GCELLCORELIBSHARED_EXPORT CSocket : public CObject
{
	Q_OBJECT
private:
    mutable QReadWriteLock m_lock;
    quint16 m_blockSize;
    QTcpSocket *m_tcpSocket;
protected:
    bool processForwardQuery(const QString &forwardQuery);
public:
    explicit CSocket(QObject *parent = 0);

    static void splitAddressPort(const QString &addressPort, QString &address, quint16 &port);

    QString hostAddress() const;
    quint16 hostPort() const;
    int state() const;
private slots:
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketError(const int &socketError);
public slots:
    void connectToHost(const QString &addressPort);
    void setSocketDescriptor(const qintptr &socketDescriptor);
    void disconnectFromHost();
signals:
    void connected(CSocket *thisSocket);
    void disconnected(CSocket *thisSocket);
    void error(CSocket *thisSocket, QString errorString, int error);
};

#endif // CTCPSOCKET_H