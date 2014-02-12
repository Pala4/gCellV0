#ifndef QUERYDESC_H
#define QUERYDESC_H

#include "gcellcorelib_global.h"

class QObject;

struct GCELLCORELIBSHARED_EXPORT QueryDesc
{
    QueryDesc() : receiver(nullptr), queryName(QString()), queryID(-1){}
    QueryDesc(QObject *_receiver, const QString &_queryName, const int &_queryID) :
        receiver(_receiver), queryName(_queryName), queryID(_queryID){}
    QObject *receiver;
    QString queryName;
    int queryID;
};

#endif // QUERYDESC_H