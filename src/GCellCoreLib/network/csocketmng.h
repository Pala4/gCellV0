#ifndef CSOCKETMNG_H
#define CSOCKETMNG_H

#include "gcellcorelib_global.h"

#include <QObject>

#include <QMap>
#include <QtNetwork/QTcpSocket>

namespace gccore {

class CSocket;

class GCELLCORELIBSHARED_EXPORT CSocketMng : public QObject
{
	Q_OBJECT
private:
    QMap<QString, CSocket*> m_sockets;

    bool addSocket(CSocket *socket, const QString &addressPort);
public:
	explicit CSocketMng(QObject *parent = 0);
private slots:
    void onSocketDisconnected(CSocket *socket);
    void onNetReceiveData(const QString &data, const int &dataType);
public slots:
    void addConnection(const qintptr &socketDescriptor);
    void disconnectFromHost(const QString &addressPort);

    void netSendData(const QString &addressPort, const QString &data, const int dataType);
signals:
    void netReceiveData(QString addressPort, QString data, int dataType);
};

}

#endif // CSOCKETMNG_H