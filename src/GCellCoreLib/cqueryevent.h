#ifndef CQUERYEVENT_H
#define CQUERYEVENT_H

#include "gcellcorelib_global.h"

#include <QObject>
#include <QEvent>

class GCELLCORELIBSHARED_EXPORT CQueryEvent : public QObject, public QEvent
{
    Q_OBJECT
public:
    enum EventType{QueryEvent = QEvent::User + 1};
private:
    int m_queryID;
    QObject *m_responsReceiver;
public:
    explicit CQueryEvent(const int &queryID = -1, QObject *responsReceiver = nullptr);

    const int& queryID() const{return m_queryID;}
    void setQueryID(const int &queryID){m_queryID = queryID;}

    QObject* responsReceiver(){return m_responsReceiver;}
    void setResponsReceiver(QObject *responsReceiver);
private slots:
    void onResponsReceiverDestroyed(){m_responsReceiver = nullptr;}
};

#endif // CQUERYEVENT_H