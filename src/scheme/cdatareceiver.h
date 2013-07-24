#ifndef CDATARECEIVER_H
#define CDATARECEIVER_H

#include "calgorithm.h"

class CDataReceiver : public CAlgorithm
{
	Q_OBJECT
public:
	explicit CDataReceiver(QGraphicsItem *parent = 0);
};

#endif // CDATARECEIVER_H