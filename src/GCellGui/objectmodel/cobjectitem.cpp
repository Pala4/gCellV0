#include "cobjectitem.h"

/*!
 * \class CObjectItem
 */
QObject* CObjectItem::parentObject(void)
{
    QObject *thisObject = toQObject();
    if (thisObject == nullptr)
        return nullptr;

    return thisObject->parent();
}

QObjectList CObjectItem::childrenObjects()
{
    QObject *thisObject = toQObject();
    if (thisObject == nullptr)
        return QObjectList();

    return thisObject->children();
}

QString CObjectItem::caption()
{
    QObject *thisObject = toQObject();
    if (thisObject == nullptr)
        return QString();

    return thisObject->objectName();
}