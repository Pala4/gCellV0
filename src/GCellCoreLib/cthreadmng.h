#ifndef CTHREADMNG_H
#define CTHREADMNG_H

#include "gcellcorelib_global.h"

#include <QObject>

class GCELLCORELIBSHARED_EXPORT CThreadMng : public QObject
{
    Q_OBJECT
public:
    explicit CThreadMng(QObject *parent = 0);
};

#endif // CTHREADMNG_H
