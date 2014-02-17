#ifndef CQUERYPARSER_H
#define CQUERYPARSER_H

#include "gcellcorelib_global.h"

#include <QStringList>

class GCELLCORELIBSHARED_EXPORT CQueryParser
{
public:
    CQueryParser(){}
    static QStringList pars(const QString &query, bool &error, QString &errorDescription);
};

#endif // CQUERYPARSER_H