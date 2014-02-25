#include "ccmdparser.h"

#include <QObject>

using namespace gccore;

QStringList CCmdParser::pars(const QString &cmdString, bool &error, QString &errorDescription)
{
    QStringList queryList;

    bool quotesMode = false;
    bool isString = false;
    int charIndex = 0;
    int opEndPos = cmdString.count() - 1;
    while (charIndex <= opEndPos) {
        QString operand;
        for (; charIndex <= opEndPos; ++charIndex) {
            if (!quotesMode) {
                if (cmdString[charIndex] == QLatin1Char('"')) {
                    quotesMode = true;
                    break;
                } else if (cmdString[charIndex] == QLatin1Char(' '))
                    break;
            }
            else if (cmdString[charIndex] == QLatin1Char('"')) {
                quotesMode = false;
                isString = true;
                break;
            }

            operand.append(cmdString[charIndex]);
        }
        ++charIndex;

        if (!isString)
            operand = operand.simplified().toLower().replace(QLatin1Char(' '), QLatin1Char('_'));

        isString = false;

        if (!operand.isEmpty())
            queryList << operand;
    }

    if (queryList.isEmpty()) {
        error = true;
        errorDescription = QObject::tr("Not a command");
        return queryList;
    }

    error = false;
    errorDescription = QString();

    return queryList;
}