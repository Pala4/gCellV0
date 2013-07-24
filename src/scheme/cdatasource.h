#ifndef CDATASOURCE_H
#define CDATASOURCE_H

#include "calgorithm.h"

class CDataSource : public CAlgorithm
{
	Q_OBJECT
public:
	explicit CDataSource(QGraphicsItem *parent = 0);
};

#endif // CDATASOURCE_H