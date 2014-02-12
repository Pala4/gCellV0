#ifndef CBASE_H
#define CBASE_H

#include "gcellcorelib_global.h"

#include "cqueryeventprocessor.h"

class CQueryEventProcessor;
class CQueryEvent;

class GCELLCORELIBSHARED_EXPORT CBase
{
private:
    CQueryEventProcessor *m_cmdEventProc;
protected:
    friend class CQueryEventProcessor;
    void initCmdEventProcessor();
    virtual void processCommand(CQueryEvent *event);
public:
    CBase();
    virtual ~CBase();

    QObject* toQObject(){return dynamic_cast<QObject*>(this);}
};

#endif // CBASE_H