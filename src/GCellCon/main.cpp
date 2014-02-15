#include <QCoreApplication>

#include "ciosystem.h"
#include "cchannel.h"
#include "cconio.h"

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

    conIO.setQuery(QStringLiteral("get_con_info"));

    return a.exec();
}
