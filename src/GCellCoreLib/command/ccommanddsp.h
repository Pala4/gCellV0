#ifndef CCOMMANDDSP_H
#define CCOMMANDDSP_H

#include "gcellcorelib_global.h"

#include <QObject>

#include <QMap>

#include "ccommand.h"

namespace gccore {

#define ERR_PARS COMMAND_LAST_ERROR + 1

class GCELLCORELIBSHARED_EXPORT CCommandDsp : public QObject
{
    Q_OBJECT
private:
    QMap<QString, CCommand*> m_commands;
public:
    explicit CCommandDsp(QObject *parent = 0);

    CCommand* registerCommand(QObject *cmdReceiver, const char *method, const QString &cmdName);
    bool registerCommand(CCommand *command);

    CCommand* command(const QString &cmdName);
public slots:
    bool execute(const QString &senderID, const QString &cmdString, QString &respons, int &errcode,
                 QString &errdesc);
};

}

#endif // CCOMMANDDSP_H