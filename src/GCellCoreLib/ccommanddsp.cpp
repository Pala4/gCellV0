#include "ccommanddsp.h"

#include "ccmdevent.h"

void CCommandDsp::setObject(QObject *object)
{
    if (object != nullptr)
        object->installEventFilter(this);
}

bool CCommandDsp::eventFilter(QObject *object, QEvent *event)
{
    if (static_cast<int>(event->type()) == static_cast<int>(CCmdEvent::CmdEvent)) {
        CCmdEvent *cmdEvent = dynamic_cast<CCmdEvent*>(event);
        if (cmdEvent != nullptr) {
            CBase *baseObject = dynamic_cast<CBase*>(object);
            if (baseObject != nullptr) {
                baseObject->processCommand(cmdEvent);
                return true;
            }
        }
    }

    return false;
}

CCommandDsp::CCommandDsp(QObject *parent) : QObject(parent)
{
    setObjectName("CCommandDsp");
}