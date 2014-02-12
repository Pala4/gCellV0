#include "cqueryevent.h"

CQueryEvent::CQueryEvent(const int &queryID) : QEvent(QEvent::None)
{
    m_queryID = queryID;

    t = CQueryEvent::QueryEvent;
    registerEventType(t);
}