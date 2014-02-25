#include <QCoreApplication>

#include "cserverapp.h"

using namespace gcserver;

int main(int argc, char *argv[])
{
    CServerApp serverApp(argc, argv);
    serverApp.start();

    return serverApp.exec();
}