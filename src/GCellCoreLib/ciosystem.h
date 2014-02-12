#ifndef CIOSYSTEM_H
#define CIOSYSTEM_H

#include "gcellcorelib_global.h"

#include <QObject>

#include "cbase.h"

#include <QMap>

#include "package.h"

class CChannel;

struct GCELLCORELIBSHARED_EXPORT CmdDesc
{
    CmdDesc() : receiver(nullptr), cmdName(QString()), cmdID(-1){}
    CmdDesc(QObject *_receiver, const QString &_cmdName, const int &_cmdID) :
        receiver(_receiver), cmdName(_cmdName), cmdID(_cmdID){}
    QObject *receiver;
    QString cmdName;
    int cmdID;
};

class GCELLCORELIBSHARED_EXPORT CIOSystem : public QObject, public CBase
{
    Q_OBJECT
private:
    QMap<QString, CmdDesc> m_cmdDescs;
    QMap<int, CChannel*> m_channels;

    void execCommand(const QString &cmdName);
    int generateChannelID();
public:
    explicit CIOSystem(QObject *parent = 0);

    CmdDesc registerCommand(QObject *receiver, const QString &cmdName, const int &cmdID);

    CChannel* createChannel();    
private slots:
    void onCmdReceiverDestroyed(QObject *objReceiver);
    void onChannelDestroyed(QObject *objChannel);
public slots:
    void sendBackwardCmd(Package pkg);
    void sendBackwardMsg(Package pkg);
    void sendForwardCmd(const int &channelID, const QString &cmd);
    void sendForwardMsg(const int &channelID, const QString &msg);
signals:
    void backwardCmd(QString cmd);
    void backwardMsg(QString msg);
    void forwardCmd(Package pkg);
    void forwardMsg(Package pkg);
};

#endif // CIOSYSTEM_H