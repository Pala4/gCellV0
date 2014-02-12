#include <QCoreApplication>

#include "ciosystem.h"
#include "cchannel.h"
#include "cconio.h"
#include "cthreadmng.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CConIO conIO;
    conIO.start();

    CIOSystem ioSystem;
    ioSystem.setObjectName(QStringLiteral("ioSystem"));
    ioSystem.registerQueryDesc(&conIO, QStringLiteral("GetConIOInfo"), (int)CConIO::GetInfo);

    CChannel *channel = ioSystem.createChannel();
    QObject::connect(&conIO, SIGNAL(sendCmd(QString)), channel, SLOT(sendForwardCmd(QString)));
    QObject::connect(channel, SIGNAL(backwardMsg(QString)), &conIO, SLOT(setMsg(QString)));

    conIO.setCmd(QStringLiteral("GetConIOInfo"));
    conIO.setCmd(QStringLiteral("GetChannelInfo"));
    conIO.setCmd(QStringLiteral("GetIOSystemInfo"));

    CThreadMng threadMng;
    threadMng.setObjectName(QStringLiteral("threadMng"));
    QObject::connect(&ioSystem, SIGNAL(forwardCmd(Package)), &threadMng, SLOT(setCmd(Package)));
    QObject::connect(&ioSystem, SIGNAL(forwardMsg(Package)), &threadMng, SLOT(setMsg(Package)));
    QObject::connect(&threadMng, SIGNAL(backwardCmd(Package)),
                     &ioSystem, SLOT(sendBackwardCmd(Package)));
    QObject::connect(&threadMng, SIGNAL(backwardMsg(Package)),
                     &ioSystem, SLOT(sendBackwardMsg(Package)));

    int ret = a.exec();

    return ret;
}