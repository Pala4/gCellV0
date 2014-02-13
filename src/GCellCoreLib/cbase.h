#ifndef CBASE_H
#define CBASE_H

#include "gcellcorelib_global.h"

#include "cqueryeventprocessor.h"
#include "querydesc.h"

class CQueryEventProcessor;
class CQueryEvent;

class GCELLCORELIBSHARED_EXPORT CBase
{
private:
    CQueryEventProcessor *m_cmdEventProc;
    int m_test;
protected:
    friend class CQueryEventProcessor;
    void initCmdEventProcessor();
    virtual void processCommand(CQueryEvent *event);

    void sendRespons(QueryDesc queryDesc, const QString &respons);
public:
    CBase();
    virtual ~CBase();

    QObject* toQObject(){return dynamic_cast<QObject*>(this);}
};

#endif // CBASE_H
