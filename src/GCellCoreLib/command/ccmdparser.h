#ifndef CCMDPARSER_H
#define CCMDPARSER_H

#include "gcellcorelib_global.h"

#include <QStringList>

namespace gccore {

class GCELLCORELIBSHARED_EXPORT CCmdParser
{
public:
    CCmdParser(){}
    static QStringList pars(const QString &cmdString, bool &error, QString &errorDescription);
};

}

#endif // CQUERYPARSER_H