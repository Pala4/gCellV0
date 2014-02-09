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
    void sendBackwardCmd(const QString &cmd);
    void sendBackwardMsg(const QString &msg);
    void sendForwardCmd(const QString &cmd);
    void sendForwardMsg(const QString &msg);
signals:
    void backwardCmd(QString cmd);
    void backwardMsg(QString msg);
    void forwardCmd(int channelID, QString cmd);
    void forwardMsg(int channelID, QString msg);
};

#endif // CCHANNEL_H