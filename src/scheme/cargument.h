#ifndef CARGUMENT_H
#define CARGUMENT_H

#include "cportal.h"

class CArgument : public CPortal
{
	Q_OBJECT
public:
	explicit CArgument(QGraphicsItem *parent = 0);
};

#endif // CARGUMENT_H