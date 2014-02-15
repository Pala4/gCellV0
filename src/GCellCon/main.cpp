#include <QCoreApplication>

#include "ciosystem.h"
#include "cchannel.h"
#include "cconio.h"
#include "cthreadmng.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CConIO *conIO = new CConIO();
    conIO->start();

    CIOSystem *ioSystem = new CIOSystem();
    ioSystem->setObjectName(QStringLiteral("ioSystem"));
    ioSystem->registerTransaction(conIO, "get_con_info", (int)CConIO::GetInfo);

    CChannel *channel = ioSystem->createChannel();
    QObject::connect(conIO, SIGNAL(sendQuery(QString)), channel, SLOT(sendForwardQuery(QString)), Qt::QueuedConnection);
    QObject::connect(channel, SIGNAL(backwardRespons(QString)), conIO, SLOT(setRespons(QString)));

    conIO->setQuery(QStringLiteral("get_con_info"));

    CThreadMng threadMng;
    threadMng.setObjectName(QStringLiteral("threadMng"));
//    QObject::connect(&ioSystem, SIGNAL(forwardCmd(Package)), &threadMng, SLOT(setCmd(Package)));
//    QObject::connect(&ioSystem, SIGNAL(forwardMsg(Package)), &threadMng, SLOT(setMsg(Package)));
//    QObject::connect(&threadMng, SIGNAL(backwardCmd(Package)),
//                     &ioSystem, SLOT(sendBackwardCmd(Package)));
//    QObject::connect(&threadMng, SIGNAL(backwardMsg(Package)),
//                     &ioSystem, SLOT(sendBackwardMsg(Package)));

    int ret = a.exec();

    delete ioSystem;
    delete conIO;

    return ret;
}
