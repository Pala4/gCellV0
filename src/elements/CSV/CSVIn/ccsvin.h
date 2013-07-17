#ifndef CCSVIN_H
#define CCSVIN_H

#include "../../../core/scheme/celement.h"

class CCSVIn : public CElement
{
    Q_OBJECT
public:
	Q_INVOKABLE explicit CCSVIn(QObject *parent = 0);
};

#endif // CCSVIN_H