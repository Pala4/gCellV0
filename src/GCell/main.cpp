#include <QApplication>

#include "mainwindow/cmainwindow.h"

using namespace gcell;
using namespace mainwindow;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	app.setQuitOnLastWindowClosed(true);

	CMainWindow mainWindow;
	mainWindow.show();

    return app.exec();
}