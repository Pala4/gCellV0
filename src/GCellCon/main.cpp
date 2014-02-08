#include <QCoreApplication>

#include "cconio.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CConIO conIO;
    conIO.start();

    int ret = a.exec();

    return ret;
}