#ifndef CGRLINK_H
#define CGRLINK_H

#include <QObject>
#include <QGraphicsPathItem>

class CGrLink : public QObject, public QGraphicsPathItem
{
    Q_OBJECT
public:
    explicit CGrLink(QGraphicsItem *parent = 0);
};

#endif // CGRLINK_H