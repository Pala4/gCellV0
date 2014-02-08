#ifndef COBJECTITEM_H
#define COBJECTITEM_H

#include <QObject>

class CObjectItem
{
public:
    explicit CObjectItem(){}
    virtual ~CObjectItem(){}

    virtual QObject* parentObject();
    virtual QObjectList childrenObjects();
    virtual QString caption();

    QObject* toQObject(){return dynamic_cast<QObject*>(this);}
};

#endif // COBJECTITEM_H