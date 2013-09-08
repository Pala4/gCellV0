#ifndef CLINKENV_H
#define CLINKENV_H

#include <QPointF>
#include <QRectF>

#include "../portal/cportal.h"

class CLink;

class CLinkEnv
{
private:
	CLink *m_link;

	QPointF fixedPoint(const QPointF &basePoint, const CPortal::TPortalOrientation &portalOrientation);
public:
	CLinkEnv(void) : m_link(0){}
	CLinkEnv(CLink *link = 0);

	CLink* link(void){return m_link;}
	void setLink(CLink *link){m_link = link;}

	qreal fixedOffset(void) const;

	QPointF firstPoint(void) const;
	CPortal::TPortalOrientation firstPortalOrientation(void) const;
	QRectF firstFigure(void) const;

	QPointF secondPoint(void) const;
	CPortal::TPortalOrientation secondPortalOrientation(void) const;
	QRectF secondFigure(void) const;

	QPointF firstFixedPoint(void){return fixedPoint(firstPoint(), firstPortalOrientation());}
	QPointF secondFixedPoint(void){return fixedPoint(secondPoint(), secondPortalOrientation());}

	QRectF firstFixedFigure(void){return firstFigure().adjusted(-fixedOffset(), -fixedOffset(), fixedOffset(), fixedOffset());}
	QRectF secondFixedFigure(void){return secondFigure().adjusted(-fixedOffset(), -fixedOffset(), fixedOffset(), fixedOffset());}

	QPointF massCenter(void);
};

#endif // CLINKENV_H