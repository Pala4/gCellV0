#ifndef CARGUMENT_H
#define CARGUMENT_H

#include "cportal.h"

class CArgument : public CPortal
{
	Q_OBJECT
protected:
	virtual QPainterPath calcPortalForm(void);
public:
	explicit CArgument(QGraphicsItem *parent = 0);

	virtual void calc(const int &timeFrame);
};

#endif // CARGUMENT_H