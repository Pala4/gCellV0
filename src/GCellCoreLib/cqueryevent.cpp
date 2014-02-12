#include "cqueryevent.h"

CQueryEvent::CQueryEvent(const int &queryID, QObject *responsReceiver) :
    QObject(0), QEvent(QEvent::None)
{
    m_queryID = queryID;
    m_responsReceiver = nullptr;

    setResponsReceiver(responsReceiver);

    t = CQueryEvent::QueryEvent;
    registerEventType(t);
}

void CQueryEvent::setResponsReceiver(QObject *responsReceiver)
{
    if ((m_responsReceiver != nullptr) && (m_responsReceiver == responsReceiver))
        return;
    if (m_responsReceiver != nullptr) {
        disconnect(m_responsReceiver, SIGNAL(destroyed(QObject*)),
                   this, SLOT(onResponsReceiverDestroyed()));
    }

    m_responsReceiver = responsReceiver;

    if (m_responsReceiver) {
        connect(m_responsReceiver, SIGNAL(destroyed(QObject*)),
                this, SLOT(onResponsReceiverDestroyed()));
    }
}