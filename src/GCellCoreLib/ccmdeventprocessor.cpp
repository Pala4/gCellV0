#include "ccmdeventprocessor.h"

#include "cqueryevent.h"

void CCmdEventProcessor::setObject(QObject *object)
{
    if (object != nullptr)
        object->installEventFilter(this);
}

bool CCmdEventProcessor::eventFilter(QObject *object, QEvent *event)
{
    if (static_cast<int>(event->type()) == static_cast<int>(CQueryEvent::QueryEvent)) {
        CQueryEvent *queryEvent = dynamic_cast<CQueryEvent*>(event);
        if (queryEvent != nullptr) {
            CBase *baseObject = dynamic_cast<CBase*>(object);
            if (baseObject != nullptr) {
                baseObject->processCommand(queryEvent);
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