#include "ccmdeventprocessor.h"

#include "ccmdevent.h"

void CCmdEventProcessor::setObject(QObject *object)
{
    if (object != nullptr)
        object->installEventFilter(this);
}

bool CCmdEventProcessor::eventFilter(QObject *object, QEvent *event)
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

CCmdEventProcessor::CCmdEventProcessor(QObject *parent) : QObject(parent)
{
    setObjectName("CCommandDsp");
}