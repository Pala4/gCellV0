#include "cmockcmdreceiver.h"

#include "command/ccommand.h"

using namespace gccore;

CMockCmdReceiver::CMockCmdReceiver(QObject *parent) : QObject(parent)
{
    setObjectName(QStringLiteral("CMockCmdReceiver"));
}

bool CMockCmdReceiver::testCmdNoArgs(CCommand *cmd)
{
    if (cmd == nullptr)
        return false;

    cmd->setRespons("Success");
    return true;
}

bool CMockCmdReceiver::testCmd2Args(CCommand *cmd, const QString &stringArg1, const int &intArg2)
{
    if (cmd == nullptr)
        return false;

    cmd->setRespons(QString("Success: %1 %2").arg(stringArg1).arg(intArg2));
    return true;
}

bool CMockCmdReceiver::testCmd3Args(CCommand *cmd, const bool &boolArg1, const qreal &qrealArg2,
                                    const QString &stringArg3)
{
    if (cmd == nullptr)
        return false;

    QString boolArg1Str = boolArg1 ? QStringLiteral("true") : QStringLiteral("false");

    cmd->setRespons(QString("Success: %1 %2 %3").arg(boolArg1Str).arg(qrealArg2)
                    .arg(stringArg3));
    return true;
}