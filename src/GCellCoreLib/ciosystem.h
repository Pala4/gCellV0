#ifndef CIOSYSTEM_H
#define CIOSYSTEM_H

#include "gcellcorelib_global.h"

#include <QObject>

#include "cbase.h"

#include <QMap>

#include "package.h"

class CChannel;

struct GCELLCORELIBSHARED_EXPORT QueryDesc
{
    QueryDesc() : receiver(nullptr), queryName(QString()), queryID(-1){}
    QueryDesc(QObject *_receiver, const QString &_queryName, const int &_queryID) :
        receiver(_receiver), queryName(_queryName), queryID(_queryID){}
    QObject *receiver;
    QString queryName;
    int queryID;
};

class GCELLCORELIBSHARED_EXPORT CIOSystem : public QObject, public CBase
{
    Q_OBJECT
private:
    QMap<QString, QueryDesc> m_queryDescs;
    QMap<int, CChannel*> m_channels;

    void sendQuery(const QString &queryName);
    int generateChannelID();
public:
    explicit CIOSystem(QObject *parent = 0);

    QueryDesc registerCommand(QObject *receiver, const QString &queryName, const int &queryID);

    CChannel* createChannel();    
private slots:
    void onQueryReceiverDestroyed(QObject *objReceiver);
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