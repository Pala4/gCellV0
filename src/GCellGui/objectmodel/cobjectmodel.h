#ifndef COBJECTMODEL_H
#define COBJECTMODEL_H

#include <QAbstractItemModel>

class CObjectItem;

class CObjectModel : public QAbstractItemModel
{
    Q_OBJECT
private:
    QList<QObject*> m_roots;

    void addObject(QObject *object);
    void removeObject(QObject *object);
    void addObjectTree(QObject *node);
    void removeObjectTree(QObject *node);
protected:
    QModelIndex objectToIndex(QObject *object);

    bool eventFilter(QObject *watched, QEvent *event);
public:
    explicit CObjectModel(QObject *parent = 0);

    int	rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QModelIndex	parent(const QModelIndex &index) const;
    QModelIndex	index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void setRoots(const QObjectList &roots);
    const QObjectList& roots(){return m_roots;}
private slots:
    void onObjectAdded(QObject *object);
    void onObjectRemoved(QObject *object);
};

#endif // COBJECTMODEL_H