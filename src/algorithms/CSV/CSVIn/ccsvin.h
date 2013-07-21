#ifndef CCSVIN_H
#define CCSVIN_H

#include "../../../scheme/calgorithm.h"

class CCSVIn : public CAlgorithm
{
    Q_OBJECT
public:
	Q_INVOKABLE explicit CCSVIn(QGraphicsItem *parent = 0);
};

#endif // CCSVIN_H