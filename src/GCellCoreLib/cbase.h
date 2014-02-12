#ifndef CBASE_H
#define CBASE_H

#include "gcellcorelib_global.h"

#include "ccmdeventprocessor.h"

class CCmdEventProcessor;
class CQueryEvent;

class GCELLCORELIBSHARED_EXPORT CBase
{
private:
    CCmdEventProcessor *m_cmdEventProc;
protected:
    friend class CCmdEventProcessor;
    void initCmdEventProcessor();
    virtual void processCommand(CQueryEvent *event);
public:
    CBase();
    virtual ~CBase();

    QObject* toQObject(){return dynamic_cast<QObject*>(this);}
};

#endif // CBASE_H