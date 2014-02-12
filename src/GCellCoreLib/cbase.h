#ifndef CBASE_H
#define CBASE_H

#include "gcellcorelib_global.h"

#include "ccommanddsp.h"

class CCommandDsp;

class GCELLCORELIBSHARED_EXPORT CBase
{
private:
    CCommandDsp *m_commandDsp;
protected:
    friend class CCommandDsp;
    void initCommandDsp();
    virtual void processCommand(QEvent *event);
public:
    CBase();
    virtual ~CBase();

    QObject* toQObject(){return dynamic_cast<QObject*>(this);}
};

#endif // CBASE_H