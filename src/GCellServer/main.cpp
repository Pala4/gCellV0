#include <QCoreApplication>

#include "network/cserver.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    CServer tcpServer;
    qDebug() << tcpServer.start("127.0.0.1", 3353);

    int ret = app.exec();

    qDebug() << tcpServer.stop();

    return ret;
}