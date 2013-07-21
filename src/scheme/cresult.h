#ifndef CRESULT_H
#define CRESULT_H

#include "cportal.h"

class CResult : public CPortal
{
	Q_OBJECT
public:
	explicit CResult(QGraphicsItem *parent = 0);
};

#endif // CRESULT_H