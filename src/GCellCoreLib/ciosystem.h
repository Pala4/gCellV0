#ifndef CIOSYSTEM_H
#define CIOSYSTEM_H

#include "gcellcorelib_global.h"

#include <QObject>

#include <QMap>

class CChannel;

class GCELLCORELIBSHARED_EXPORT CIOSystem : public QObject
{
    Q_OBJECT
private:
    QMap<int, CChannel*> m_channels;

    int generateChannelID();
public:
    explicit CIOSystem(QObject *parent = 0);

    CChannel* createChannel();
};

#endif // CIOSYSTEM_H