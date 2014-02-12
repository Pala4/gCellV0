#ifndef CCMDEVENTPROCESSOR_H
#define CCMDEVENTPROCESSOR_H

#include "gcellcorelib_global.h"

#include <QObject>

#include "cbase.h"

class GCELLCORELIBSHARED_EXPORT CCmdEventProcessor : public QObject
{
    Q_OBJECT
private:
    friend class CBase;
    void setObject(QObject *object);
protected:
    bool eventFilter(QObject *object, QEvent *event);
public:
    explicit CCmdEventProcessor(QObject *parent = 0);
};

#endif // CCMDEVENTPROCESSOR_H