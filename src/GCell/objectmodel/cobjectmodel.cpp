#include "cobjectmodel.h"

#include <QChildEvent>

#include "cobjectitem.h"

void CObjectModel::addObject(QObject *object)
{
    if (object == nullptr)
        return;

    object->installEventFilter(this);
}

void CObjectModel::removeObject(QObject *object)
{
    if (object == nullptr)
        return;

    object->removeEventFilter(this);
}

void CObjectModel::addObjectTree(QObject *node)
{
    if (node != nullptr) {
        addObject(node);

        QObjectList childList;
        CObjectItem *nodeItem = dynamic_cast<CObjectItem*>(node);
        if (nodeItem != nullptr)
            childList = nodeItem->childrenObjects();
        else
            childList = node->children();

        foreach (QObject *child, childList) {
            if (child != nullptr)
                addObjectTree(child);
        }
    }
}

void CObjectModel::removeObjectTree(QObject *node)
{
    if (node != nullptr) {
        removeObject(node);

        QObjectList childList;
        CObjectItem *nodeItem = dynamic_cast<CObjectItem*>(node);
        if (nodeItem != nullptr)
            childList = nodeItem->childrenObjects();
        else
            childList = node->children();

        foreach (QObject *child, childList) {
            if (child != nullptr)
                removeObjectTree(child);
        }
    }
}

QModelIndex CObjectModel::objectToIndex(QObject *object)
{
    if (object == nullptr)
        return QModelIndex();

    int objectCol = 0;

    if (m_roots.contains(object))
        return createIndex(m_roots.indexOf(object), objectCol, object);

    QObject *parentObject = 0;
    CObjectItem *objectItem = dynamic_cast<CObjectItem*>(object);
    if (objectItem != nullptr) {
        parentObject = objectItem->parentObject();
    } else {
        parentObject = object->parent();
    }

    int objectRow = -1;
    if (parentObject != nullptr) {
        CObjectItem *parentItem = dynamic_cast<CObjectItem*>(parentObject);
        if (parentItem != nullptr)
            objectRow = parentItem->childrenObjects().indexOf(object);
        else
            objectRow = parentObject->children().indexOf(object);
    }

    if (objectRow >= 0)
        return createIndex(objectRow, objectCol, object);

    return QModelIndex();
}

bool CObjectModel::eventFilter(QObject *watched, QEvent *event)
{
    QChildEvent *childEvent = dynamic_cast<QChildEvent*>(event);
    if (childEvent != nullptr) {
        if (childEvent->added())
            onObjectAdded(childEvent->child());
        else if (childEvent->removed())
            onObjectRemoved(childEvent->child());
    }

    return QAbstractItemModel::eventFilter(watched, event);
}

CObjectModel::CObjectModel(QObject *parent) : QAbstractItemModel(parent)
{
    setObjectName("CObjectModel");
}

int CObjectModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return m_roots.count();

    QObject *object = static_cast<QObject*>(parent.internalPointer());
    if (object == nullptr)
        return 0;

    CObjectItem *objectItem = dynamic_cast<CObjectItem*>(object);
    if (objectItem != nullptr)
        return objectItem->childrenObjects().count();

    return object->children().count();
}

int CObjectModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QModelIndex CObjectModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    int parentCol = 0;

    QObject *childObject = static_cast<QObject*>(index.internalPointer());
    if (childObject == nullptr)
        return QModelIndex();

    QObject *parentObject = 0;
    CObjectItem *childItem = dynamic_cast<CObjectItem*>(childObject);
    if (childItem != nullptr) {
        parentObject = childItem->parentObject();
    } else {
        parentObject = childObject->parent();
    }
    if (parentObject == nullptr)
        return QModelIndex();

    if (m_roots.contains(parentObject))
        return createIndex(m_roots.indexOf(parentObject), parentCol, parentObject);

    QObject *parentParentObject = 0;
    CObjectItem *parentItem = dynamic_cast<CObjectItem*>(parentObject);
    if (parentItem != nullptr) {
        parentParentObject = parentItem->parentObject();
    } else {
        parentParentObject = parentObject->parent();
    }

    int parentRow = -1;
    if (parentParentObject != nullptr) {
        CObjectItem *parentParentItem = dynamic_cast<CObjectItem*>(parentParentObject);
        if (parentParentItem != nullptr)
            parentRow = parentParentItem->childrenObjects().indexOf(parentObject);
        else
            parentRow = parentParentObject->children().indexOf(parentObject);
    }

    if (parentRow >= 0)
        return createIndex(parentRow, parentCol, parentObject);

    return QModelIndex();
}

QModelIndex CObjectModel::index(int row, int column, const QModelIndex &parent) const
{
    if ((row < 0) || (column < 0))
        return QModelIndex();

    if (!parent.isValid()) {
        if (row >= m_roots.count())
            return QModelIndex();

        return createIndex(row, column, m_roots.at(row));
    } else {
        QObject *parentObject = static_cast<QObject*>(parent.internalPointer());
        if (parentObject == nullptr)
            return QModelIndex();

        QObject *childObject = 0;
        CObjectItem *parentItem = dynamic_cast<CObjectItem*>(parentObject);
        if (parentItem != nullptr) {
            if (row >= parentItem->childrenObjects().count())
                return QModelIndex();

            childObject = parentItem->childrenObjects().at(row);
        } else {
            if (row >= parentObject->children().count())
                return QModelIndex();

            childObject = parentObject->children().at(row);
        }

        return createIndex(row, column, childObject);
    }

    return QModelIndex();
}

QVariant CObjectModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    QObject *object = static_cast<QObject*>(index.internalPointer());
    if (object == nullptr)
        return QVariant();

    CObjectItem *objectItem = dynamic_cast<CObjectItem*>(object);

    switch (role) {
    case Qt::DisplayRole:
        if (objectItem) {
            return QVariant(objectItem->caption().isEmpty() ? tr("Unknown object!")
                                                            : objectItem->caption());
        } else {
            return QVariant(object->objectName().isEmpty() ? tr("Unknown object!")
                                                           : object->objectName());
        }
    }

    return QVariant();
}

void CObjectModel::setRoots(const QObjectList &roots)
{
    beginResetModel();

    foreach (QObject *root, m_roots) {
        if (root != nullptr)
            removeObjectTree(root);
    }
    m_roots.clear();

    m_roots = roots;

    foreach (QObject *root, m_roots) {
        if (root != nullptr)
            addObjectTree(root);
    }

    endResetModel();
}

void CObjectModel::onObjectAdded(QObject *object)
{
    QModelIndex parentIdx;

    CObjectItem *objectItem = dynamic_cast<CObjectItem*>(object);
    if (objectItem != nullptr)
        parentIdx = objectToIndex(objectItem->parentObject());
    else if (object != nullptr)
        parentIdx = objectToIndex(object->parent());

    QModelIndex childIdx = objectToIndex(object);

    if (childIdx.isValid())
        emit beginInsertRows(parentIdx, childIdx.row(), childIdx.row());
    else
        beginResetModel();

    addObjectTree(object);

    if (childIdx.isValid())
        emit endInsertRows();
    else
        endResetModel();
}

void CObjectModel::onObjectRemoved(QObject *object)
{
    QModelIndex parentIdx;

    CObjectItem *objectItem = dynamic_cast<CObjectItem*>(object);
    if (objectItem != nullptr)
        parentIdx = objectToIndex(objectItem->parentObject());
    else if (object != nullptr)
        parentIdx = objectToIndex(object->parent());

    QModelIndex childIdx = objectToIndex(object);

    if (childIdx.isValid())
        emit beginRemoveRows(parentIdx, childIdx.row(), childIdx.row());
    else
        beginResetModel();

    removeObjectTree(object);

    if (childIdx.isValid())
        emit endRemoveRows();
    else
        endResetModel();
}