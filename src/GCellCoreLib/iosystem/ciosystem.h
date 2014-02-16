#ifndef CIOSYSTEM_H
#define CIOSYSTEM_H

#include "gcellcorelib_global.h"

#include <QObject>

#include "cobject.h"

#include <QMap>

class CChannel;

class GCELLCORELIBSHARED_EXPORT CIOSystem : public QObject, public CObject
{
    Q_OBJECT
private:
    QMap<QString, CTransaction*> m_transactions;
    QMap<int, CChannel*> m_channels;

    int generateChannelID();
protected:
    void processTransaction(CTransaction *transaction);
public:
    explicit CIOSystem(QObject *parent = 0);

    CTransaction* registerTransaction(QObject *queryReceiver, const QString &query,
                                      const int &cmdID);

    CChannel* createChannel();    
private slots:
    void onChannelDestroyed(QObject *objChannel);
    void onTransactionDestroyed(QObject *objTransaction);
public slots:
    void sendBackwardQuery(const int &channelID, const QString &query);
    void sendBackwardRespons(const int &channelID, const QString &respons);
    void sendForwardQuery(const int &channelID, const QString &query);
    void sendForwardRespons(const int &channelID, const QString &respons);
signals:
    void backwardQuery(QString query);
    void backwardRespons(QString respons);
    void forwardQuery();
    void forwardRespons();
};

#endif // CIOSYSTEM_H
