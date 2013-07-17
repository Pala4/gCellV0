#ifndef CLINK_H
#define CLINK_H

#include <QObject>

class CLink : public QObject
{
    Q_OBJECT
public:
    explicit CLink(QObject *parent = 0);
};

#endif // CLINK_H