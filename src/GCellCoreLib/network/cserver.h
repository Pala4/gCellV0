#ifndef CSERVER_H
#define CSERVER_H

#include "gcellcorelib_global.h"

#include <QtNetwork/QTcpServer>

class CTcpServer : public QTcpServer
{
    Q_OBJECT
protected:
    virtual void incomingConnection(qintptr socketDescriptor);
public:
    explicit CTcpServer(QObject *parent = 0);
signals:
    void hasIncomingConnection(qintptr socketDescriptor);
};

class GCELLCORELIBSHARED_EXPORT CServer : public QObject
{
	Q_OBJECT
private:
    CTcpServer *m_tcpServer;
public:
    explicit CServer(QObject *parent = 0);
public slots:
    QString start(const QString &address, const quint16 &port);
    QString stop();
private slots:
    void onTcpServerDestroyed();
signals:
	void newConnection(qintptr socketDescriptor);
};

#endif // CSERVER_H