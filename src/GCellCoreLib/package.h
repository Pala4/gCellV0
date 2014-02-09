#ifndef CPACKAGE_H
#define CPACKAGE_H

#include "gcellcorelib_global.h"

#include <QString>
#include <QPointer>

class CQuery;

struct GCELLCORELIBSHARED_EXPORT Package
{
    int channelID;
    QString respons;
    QPointer<CQuery> query;
};

#endif // CPACKAGE_H