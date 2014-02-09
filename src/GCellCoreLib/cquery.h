#ifndef CQUERY_H
#define CQUERY_H

#include <QObject>

class CQuery : public QObject
{
    Q_OBJECT
public:
    explicit CQuery(QObject *parent = 0);
};

#endif // CQUERY_H