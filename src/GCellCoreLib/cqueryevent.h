#ifndef CQUERYEVENT_H
#define CQUERYEVENT_H

#include "gcellcorelib_global.h"

#include <QEvent>

#include "querydesc.h"

class GCELLCORELIBSHARED_EXPORT CQueryEvent : public QEvent
{
public:
    enum EventType{QueryEvent = QEvent::User + 1};
private:
    QueryDesc m_queryDesc;
public:
    explicit CQueryEvent(const QueryDesc &queryDesc);

    const QueryDesc& queryDesc() const{return m_queryDesc;}
    void setQueryDesc(const QueryDesc &queryDesc){m_queryDesc = queryDesc;}
};

#endif // CQUERYEVENT_H