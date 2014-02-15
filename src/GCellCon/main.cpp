#include <QCoreApplication>

#include "iosystem/ciosystem.h"
#include "iosystem/cchannel.h"
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
    ioSystem.registerTransaction(&conIO, QStringLiteral("get_con_info"), (int)CConIO::GetInfo);
    ioSystem.registerTransaction(&conIO, QStringLiteral("halt"), (int)CConIO::Halt);

    CChannel *channel = ioSystem.createChannel();
    QObject::connect(&conIO, SIGNAL(sendQuery(QString)), channel, SLOT(sendForwardQuery(QString)));
    QObject::connect(channel, SIGNAL(backwardRespons(QString)), &conIO, SLOT(setRespons(QString)));

    CThreadedSocket *socket = new CThreadedSocket();
    socket->connectToHost("127.0.0.1:3353");
    delete socket;

    conIO.setQuery(QStringLiteral("get_con_info"));

    int ret = a.exec();

    return ret;
}
