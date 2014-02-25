#ifndef CSERVERAPP_H
#define CSERVERAPP_H

#include <QCoreApplication>

namespace gccore {
class CCommand;
class CCommandDsp;
class CServer;
class CSocketMng;
}

namespace gcserver {

class CServerApp : public QCoreApplication
{
    Q_OBJECT
private:
    gccore::CCommandDsp *m_commandDsp;
    gccore::CServer *m_server;
    gccore::CSocketMng *m_socketMng;
public:
    explicit CServerApp(int &argc, char **argv);

    void start();
private slots:
    void onReceiveCmdString(const QString &senderID, const QString &cmdString);
    void setMessage(const QString &receiverID, const QString &msg);
    void onAboutToQuit();
    void onNetReceiveData(const QString &senderID, const QString &data, const int &dataType);

    bool cmdDisconnect(gccore::CCommand *cmd);
    bool cmdSrvInfo(gccore::CCommand *cmd);
    bool cmdHalt(gccore::CCommand *cmd);
};

}

#endif // CSERVERAPP_H