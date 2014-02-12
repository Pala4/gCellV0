#ifndef QUERYDESC_H
#define QUERYDESC_H

#include "gcellcorelib_global.h"

#include <QString>

class QObject;

struct GCELLCORELIBSHARED_EXPORT QueryDesc
{
    QueryDesc() : queryReceiver(nullptr), responsReceiver(nullptr), channelID(-1),
        queryName(QString()), responsMsg(QString()), queryID(-1){}
    QueryDesc(QObject *_queryReceiver, QObject *_responsReceiver, const int &_channelID,
              const QString &_queryName, const QString &_responsMsg, const int &_queryID) :
        queryReceiver(_queryReceiver), responsReceiver(_responsReceiver), channelID(_channelID),
        queryName(_queryName), responsMsg(_responsMsg), queryID(_queryID){}

    QObject *queryReceiver;
    QObject *responsReceiver;
    int channelID;
    QString queryName;
    QString responsMsg;
    int queryID;
};

#endif // QUERYDESC_H