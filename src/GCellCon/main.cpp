#include <QCoreApplication>

#include "ciosystem.h"
#include "cchannel.h"
#include "cconio.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CConIO conIO;
    conIO.start();

    CIOSystem *ioSystem = new CIOSystem();
    ioSystem->setObjectName(QLatin1String("ioSystem"));

    CChannel *channel = ioSystem->createChannel();
    QObject::connect(&conIO, SIGNAL(sendCmd(QString)), channel, SLOT(sendForwardCmd(QString)));
    QObject::connect(channel, SIGNAL(backwardMsg(QString)), &conIO, SLOT(setMsg(QString)));

    conIO.setCmd("GetChannelInfo");
    conIO.setCmd("GetIOSystemInfo");

    int ret = a.exec();

    delete ioSystem;

    return ret;
}