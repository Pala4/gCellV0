#include <QApplication>

#include "gui/mainwindow/cmainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    CMainWindow mainWindow;
    mainWindow.show();
    
    return app.exec();
}