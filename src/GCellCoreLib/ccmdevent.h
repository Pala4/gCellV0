#ifndef CCMDEVENT_H
#define CCMDEVENT_H

#include "gcellcorelib_global.h"

#include <QEvent>

class GCELLCORELIBSHARED_EXPORT CCmdEvent : public QEvent
{
public:
    enum EventType{CmdEvent = QEvent::User + 1};
private:
    int m_cmdID;
public:
    explicit CCmdEvent(const int &cmdID = -1);

    const int& cmdID() const{return m_cmdID;}
    void setCmdID(const int &cmdID){m_cmdID = cmdID;}
};

#endif // CCMDEVENT_H