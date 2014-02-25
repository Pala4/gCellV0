#ifndef CCONSOLEAPP_H
#define CCONSOLEAPP_H

#include <QCoreApplication>

namespace gccore {
class CCommand;
class CCommandDsp;
class CSocket;
}

namespace gcconclient {

class CConIO;

class CConsoleApp : public QCoreApplication
{
    Q_OBJECT
private:
    gccore::CCommandDsp *m_commandDsp;
    CConIO *m_conIO;
    gccore::CSocket *m_socket;
public:
    explicit CConsoleApp(int &argc, char **argv);
private slots:
    void onReceiveCmdString(const QString &cmdString);
    void setMessage(const QString &msg);
    void onAboutToQuit();
    void onNetReceiveData(const QString &data, const int &dataType);

    bool cmdNetSendMessage(gccore::CCommand *cmd, const QString &msg);
    bool cmdNetSendCommand(gccore::CCommand *cmd, const QString &cmdString);
};

}

#endif // CCONSOLEAPP_H