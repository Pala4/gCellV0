#ifndef CRESPONSEVENT_H
#define CRESPONSEVENT_H

#include "gcellcorelib_global.h"

#include <QEvent>

#include "querydesc.h"

class GCELLCORELIBSHARED_EXPORT CResponsEvent : public QEvent
{
public:
    enum EventType{ResponsEvent = QEvent::User + 2};
private:
    QueryDesc m_queryDesc;
public:
    explicit CResponsEvent(const QueryDesc &queryDesc = QueryDesc());

    const QueryDesc& queryDesc() const{return m_queryDesc;}
    void setRespons(const QueryDesc &queryDesc){m_queryDesc = queryDesc;}
};

#endif // CRESPONSEVENT_H
