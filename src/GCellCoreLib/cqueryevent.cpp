#include "cqueryevent.h"

CQueryEvent::CQueryEvent(const QueryDesc &queryDesc) : QEvent(QEvent::None)
{
    m_queryDesc = queryDesc;

    t = CQueryEvent::QueryEvent;
    registerEventType(t);
}