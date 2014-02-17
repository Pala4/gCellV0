#include "cconsoleapp.h"

int main(int argc, char *argv[])
{
    CConsoleApp conApp(argc, argv);
    conApp.start();

    return conApp.exec();
}