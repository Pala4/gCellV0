#ifndef CCOMMANDDSP_H
#define CCOMMANDDSP_H

#include "gcellcorelib_global.h"

#include <QObject>

#include "cbase.h"

class GCELLCORELIBSHARED_EXPORT CCommandDsp : public QObject
{
    Q_OBJECT
private:
    friend class CBase;
    void setObject(QObject *object);
protected:
    bool eventFilter(QObject *object, QEvent *event);
public:
    explicit CCommandDsp(QObject *parent = 0);    
};

#endif // CCOMMANDDSP_H