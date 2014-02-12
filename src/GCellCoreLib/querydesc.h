#ifndef QUERYDESC_H
#define QUERYDESC_H

#include "gcellcorelib_global.h"

#include <QString>

class QObject;

struct GCELLCORELIBSHARED_EXPORT QueryDesc
{
    QueryDesc() : queryReceiver(nullptr), responsReceiver(nullptr), queryName(QString()),
        queryID(-1){}
    QueryDesc(QObject *_queryReceiver, QObject *_responsReceiver, const QString &_queryName,
              const int &_queryID) :
        queryReceiver(_queryReceiver), responsReceiver(_responsReceiver), queryName(_queryName),
        queryID(_queryID){}

    QObject *queryReceiver;
    QObject *responsReceiver;
    QString queryName;
    int queryID;
};

#endif // QUERYDESC_H