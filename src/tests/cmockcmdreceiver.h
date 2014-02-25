#ifndef CMOCKCMDRECEIVER_H
#define CMOCKCMDRECEIVER_H

#include <QObject>

namespace gccore {
class CCommand;
}

class CMockCmdReceiver : public QObject
{
    Q_OBJECT
public:
    explicit CMockCmdReceiver(QObject *parent = 0);
public slots:
    bool testCmdNoArgs(gccore::CCommand *cmd);
    bool testCmd2Args(gccore::CCommand *cmd, const QString &stringArg1, const int &intArg2);
    bool testCmd3Args(gccore::CCommand *cmd, const bool &boolArg1, const qreal &qrealArg2,
                      const QString &stringArg3);
};

#endif // CMOCKCMDRECEIVER_H