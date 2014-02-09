#ifndef CCHANNEL_H
#define CCHANNEL_H

#include "gcellcorelib_global.h"

#include <QObject>

class GCELLCORELIBSHARED_EXPORT CChannel : public QObject
{
    Q_OBJECT
private:
    int m_id;
public:
    explicit CChannel(const int &id = -1, QObject *parent = 0);

    const int& id() const{return m_id;}
    void setID(const int &id) {m_id = id;}
public slots:
    void outCmdIn(const QString &cmd);
    void outMsgIn(const QString &msg);

    void receiveCmd(const QString &cmd);
    void receiveMsg(const QString &msg);
signals:
    void sendCmdIn(QString cmd);
    void sendMsgIn(QString msg);
    void sendCmdOut(int channelID, QString cmd);
    void sendMsgOut(int channelID, QString msg);
};

#endif // CCHANNEL_H