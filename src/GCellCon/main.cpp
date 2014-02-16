#include <QCoreApplication>

#include "iosystem/ciosystem.h"
#include "cconio.h"
#include "network/csocket.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CConIO conIO;
    conIO.setObjectName(QStringLiteral("conIO"));
    conIO.start();

    CIOSystem ioSystem;
    ioSystem.setObjectName(QStringLiteral("ioSystem"));
    ioSystem.registerTransaction(&conIO, &conIO, QStringLiteral("get_con_info"),
                                 (int)CConIO::GetInfo);
    ioSystem.registerTransaction(&conIO, &conIO, QStringLiteral("halt"), (int)CConIO::Halt);

    CSocket *socket = new CSocket();
    conIO.connectForwardObject(socket);

    socket->connectToHost("127.0.0.1:33538");

    conIO.receiveBackwardQuery(QStringLiteral("get_con_info"));

    int ret = a.exec();
    delete socket;


    return ret;
}
