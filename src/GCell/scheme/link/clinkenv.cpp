#include "clinkenv.h"

#include "clink.h"

/*!
 * \class CLinkEnv
 */
QPointF CLinkEnv::fixedPoint(const QPointF &basePoint, const CPortal::TPortalOrientation &portalOrientation)
{
	QPointF _fixedPoint = basePoint;
	switch(portalOrientation)
	{
		case CPortal::Left: return _fixedPoint - QPointF(fixedOffset(), 0.0);
		case CPortal::Top: return _fixedPoint - QPointF(0.0, fixedOffset());
		case CPortal::Right: return _fixedPoint + QPointF(fixedOffset(), 0.0);
		case CPortal::Bottom: return _fixedPoint + QPointF(0.0, fixedOffset());
		case CPortal::NoOrientation: return _fixedPoint;
	}
	return _fixedPoint;
}

CLinkEnv::CLinkEnv(CLink *link)
{
	setLink(link);
}

qreal CLinkEnv::fixedOffset(void) const
{
	return m_link ? m_link->distFromPortal() : 0.0;
}

QPointF CLinkEnv::firstPoint(void) const
{
	return (m_link) ? m_link->firstPos() : QPointF();
}

CPortal::TPortalOrientation CLinkEnv::firstPortalOrientation(void) const
{
	return m_link ? m_link->firstOrientation() : CPortal::NoOrientation;
}

QRectF CLinkEnv::firstFigure(void) const
{
	return m_link ? m_link->firstFigure() : QRectF();
}

QPointF CLinkEnv::secondPoint(void) const
{
	return (m_link) ? m_link->secondPos() : QPointF();
}

CPortal::TPortalOrientation CLinkEnv::secondPortalOrientation(void) const
{
	return m_link ? m_link->secondOrientation() : CPortal::NoOrientation;
}

QRectF CLinkEnv::secondFigure(void) const
{
	return m_link ? m_link->secondFigure() : QRectF();
}

QPointF CLinkEnv::massCenter(void)
{
	QPointF firstTopLeft = firstFigure().topLeft();
	QPointF firstBottomRight = firstFigure().bottomRight();
	QPointF secondTopLeft = secondFigure().topLeft();
	QPointF secondBottomRight = secondFigure().bottomRight();
	QRectF bound;
	bound.setTopLeft(QPointF(qMin(firstTopLeft.x(), secondTopLeft.x()),
							 qMin(firstTopLeft.y(), secondTopLeft.y())));
	bound.setBottomRight(QPointF(qMax(firstBottomRight.x(), secondBottomRight.x()),
								 qMax(firstBottomRight.y(), secondBottomRight.y())));
	return bound.center();
}