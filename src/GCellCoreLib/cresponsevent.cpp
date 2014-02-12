#include "cresponsevent.h"

CResponsEvent::CResponsEvent(const QueryDesc &queryDesc) : QEvent(QEvent::None)
{
    m_queryDesc = queryDesc;

    t = CResponsEvent::ResponsEvent;
    registerEventType(t);
}