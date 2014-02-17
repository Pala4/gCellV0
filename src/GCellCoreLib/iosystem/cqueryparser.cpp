#include "cqueryparser.h"

QStringList CQueryParser::pars(const QString &query, bool &error, QString &errorDescription)
{
    QString simpleQuery = query.simplified().toLower();
    QStringList queryList;

    int charIndex = 0;
    int opEndPos = simpleQuery.count() - 1;
    while (charIndex <= opEndPos) {
        QString operand;
        bool quotesMode = false;
        for (; charIndex <= opEndPos; ++charIndex) {
            if (!quotesMode) {
                if (simpleQuery[charIndex] == QLatin1Char('"')) {
                    quotesMode = true;
                    continue;
                } else if (simpleQuery[charIndex] == QLatin1Char(' '))
                    break;
            }
            else if (simpleQuery[charIndex] == QLatin1Char('"')) {
                quotesMode = false;
                continue;
            }

            operand.append(simpleQuery[charIndex]);
        }
        ++charIndex;

        if (!operand.isEmpty())
            queryList << operand;
    }

    error = false;
    errorDescription = QString();

    return queryList;
}