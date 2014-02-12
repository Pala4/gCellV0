#ifndef CQUERYEVENTPROCESSOR_H
#define CQUERYEVENTPROCESSOR_H

#include "gcellcorelib_global.h"

#include <QObject>

#include "cbase.h"

class GCELLCORELIBSHARED_EXPORT CQueryEventProcessor : public QObject
{
    Q_OBJECT
private:
    friend class CBase;
    void setObject(QObject *object);
protected:
    bool eventFilter(QObject *object, QEvent *event);
public:
    explicit CQueryEventProcessor(QObject *parent = 0);
};

#endif // CQUERYEVENTPROCESSOR_H