#ifndef CCONSOLEAPP_H
#define CCONSOLEAPP_H

#include <QCoreApplication>

class CConIO;
class CIOSystem;
class CSocket;

class CConsoleApp : public QCoreApplication
{
    Q_OBJECT
private:
    CConIO *m_conIO;
    CIOSystem *m_ioSystem;
    CSocket *m_socket;
public:
    explicit CConsoleApp(int &argc, char **argv);
private slots:
    void onAboutToQuit();
public slots:
    void start();
signals:
};

#endif // CCONSOLEAPP_H
