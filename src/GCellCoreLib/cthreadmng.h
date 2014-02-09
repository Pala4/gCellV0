#ifndef CTHREADMNG_H
#define CTHREADMNG_H

#include "gcellcorelib_global.h"

#include <QObject>

#include "package.h"

class GCELLCORELIBSHARED_EXPORT CThreadMng : public QObject
{
    Q_OBJECT
public:
    explicit CThreadMng(QObject *parent = 0);
public slots:
    void setCmd(const Package &pkg);
    void setMsg(const Package &pkg);
    void sendBackwardCmd(const Package &pkg);
    void sendBackwardMsg(const Package &pkg);
signals:
    void backwardCmd(Package pkg);
    void backwardMsg(Package pkg);
};

#endif // CTHREADMNG_H