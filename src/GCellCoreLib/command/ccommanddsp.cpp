#include "ccommanddsp.h"

#include "ccmdparser.h"

using namespace gccore;

CCommandDsp::CCommandDsp(QObject *parent) : QObject(parent)
{
    setObjectName(QStringLiteral("CCommandDsp"));
}

CCommand* CCommandDsp::registerCommand(QObject *cmdReceiver, const char *method,
                                       const QString &cmdName)
{
    Q_ASSERT(cmdReceiver != nullptr);
    Q_ASSERT(method != nullptr);
    Q_ASSERT(!cmdName.isEmpty());

    CCommand *command = new CCommand(cmdReceiver, method, cmdName, this);
    command->setObjectName(QString(QStringLiteral("cmd:%1")).arg(command->cmdName()));
    if (!registerCommand(command)) {
        command->deleteLater();
        command = nullptr;
    }
    return command;
}

bool CCommandDsp::registerCommand(CCommand *command)
{
    Q_ASSERT(command != nullptr);

    if (m_commands.contains(command->cmdName())) {
        qWarning(qPrintable(tr("Command [%1] aready registered").arg(command->cmdName())));
        return false;
    }

    m_commands[command->cmdName()] = command;
    return true;
}

CCommand* CCommandDsp::command(const QString &cmdName)
{
    QString sCmd = CCommand::simpleCmdName(cmdName);
    if (sCmd.isEmpty() || !m_commands.contains(sCmd))
        return nullptr;

    return m_commands[sCmd];
}

bool CCommandDsp::execute(const QString &senderID, const QString &cmdString, QString &respons,
                          int &errcode, QString &errdesc)
{
    bool parsError = false;
    QStringList cmdList = CCmdParser::pars(cmdString, parsError, errdesc);
    if (parsError || cmdList.isEmpty()) {
        errcode = ERR_PARS;
        return false;
    }

    QString cmdName = cmdList.at(0);
    if (!m_commands.contains(cmdName)) {
        errcode = ERR_METHOD_NOT_FOUND;
        errdesc = ERR_METHOD_NOT_FOUND_STR(cmdName);
        return false;
    }
    if (m_commands[cmdName] == nullptr) {
        errcode = ERR_METHOD_NOT_VALID;
        errdesc = ERR_METHOD_NOT_VALID_STR(cmdName);
        return false;
    }

    m_commands[cmdName]->setSenderID(senderID);
    bool ret = m_commands[cmdName]->execute(cmdList, errcode, errdesc);
    respons = m_commands[cmdName]->respons();
    return ret;
}