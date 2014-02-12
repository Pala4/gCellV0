#ifndef CQUERYEVENT_H
#define CQUERYEVENT_H

#include "gcellcorelib_global.h"

#include <QEvent>

class GCELLCORELIBSHARED_EXPORT CQueryEvent : public QEvent
{
public:
    enum EventType{QueryEvent = QEvent::User + 1};
private:
    int m_queryID;
public:
    explicit CQueryEvent(const int &queryID = -1);

    const int& queryID() const{return m_queryID;}
    void setQueryID(const int &queryID){m_queryID = queryID;}
};

#endif // CQUERYEVENT_H