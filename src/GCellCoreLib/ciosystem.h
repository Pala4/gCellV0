#ifndef CIOSYSTEM_H
#define CIOSYSTEM_H

#include "gcellcorelib_global.h"

#include <QObject>

#include <QMap>

class CChannel;

class GCELLCORELIBSHARED_EXPORT CIOSystem : public QObject
{
    Q_OBJECT
private:
    QMap<int, CChannel*> m_channels;

    int generateChannelID();
public:
    explicit CIOSystem(QObject *parent = 0);

    CChannel* createChannel();
private slots:
    void onChannelDestroyed(QObject *objChannel);
public slots:
    void sendBackwardCmd(const int &channelID, const QString &cmd);
    void sendBackwardMsg(const int &channelID, const QString &msg);
    void sendForwardCmd(const int &channelID, const QString &cmd);
    void sendForwardMsg(const int &channelID, const QString &msg);
signals:
    void backwardCmd(QString cmd);
    void backwardMsg(QString msg);
    void forwardCmd(QString cmd);
    void forwardMsg(QString msg);
};

#endif // CIOSYSTEM_H