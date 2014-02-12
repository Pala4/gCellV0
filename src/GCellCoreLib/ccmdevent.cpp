#include "ccmdevent.h"

CCmdEvent::CCmdEvent(const int &cmdID) : QEvent(QEvent::None)
{
    m_cmdID = cmdID;

    t = CCmdEvent::CmdEvent;
    registerEventType(t);
}