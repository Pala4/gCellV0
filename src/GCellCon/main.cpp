#include "cconsoleapp.h"

using namespace gcconclient;

int main(int argc, char *argv[])
{
    CConsoleApp conApp(argc, argv);

    return conApp.exec();
}