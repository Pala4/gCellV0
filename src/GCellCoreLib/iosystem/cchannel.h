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
    void sendBackwardQuery(const QString &query);
    void sendBackwardRespons(const QString &respons);
    void sendForwardQuery(const QString &query);
    void sendForwardRespons(const QString &respons);
signals:
    void backwardQuery(QString query);
    void backwardRespons(QString respons);
    void forwardQuery(int channelID, QString query);
    void forwardRespons(int channelID, QString respons);
};

#endif // CCHANNEL_H
