#ifndef CDATATRANSMITTER_H
#define CDATATRANSMITTER_H

#include "calgorithm.h"

class CDataTransmitter : public CAlgorithm
{
	Q_OBJECT
public:
	explicit CDataTransmitter(QGraphicsItem *parent = 0);
};

#endif // CDATATRANSMITTER_H