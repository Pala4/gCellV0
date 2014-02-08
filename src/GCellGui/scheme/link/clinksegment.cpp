#include "clinksegment.h"

#include <QStringList>

#include "clinkenv.h"

/*!
 * \class CLinkSegment
 */
CLink* CLinkSegment::link(void)
{
	return m_linkSegments ? m_linkSegments->link() : 0;
}

CLinkSegment* CLinkSegment::segment(const CLinkSegment::Type &type)
{
	return m_linkSegments ? m_linkSegments->segment(type) : 0;
}

CLinkSegment::Orientation CLinkSegment::orientByPortal(const CPortal::TPortalOrientation &portalOrientation)
{
	switch((int)portalOrientation)
	{
		case CPortal::Left:
		case CPortal::Right:
			return CLinkSegment::Horizontal;
		case CPortal::Top:
		case CPortal::Bottom:
			return CLinkSegment::Vertical;
	}

	return CLinkSegment::NoOrientation;
}

CLinkSegment::CLinkSegment(CLinkSegments *linkSegments, const CLinkSegment::Type &type)
{
	m_type = type;
	m_orientation = CLinkSegment::NoOrientation;

	m_linkSegments = linkSegments;
}

void CLinkSegment::setLinkSegments(CLinkSegments *linkSegments)
{
	if(m_linkSegments && (m_linkSegments == linkSegments)) return;

	m_linkSegments = linkSegments;

	update();
}

QPointF CLinkSegment::managedToPoint(const stManagedCoord &managedCoord)
{
	if(!link()) return QPointF();

	CLinkEnv *linkEnv = link()->linkEnv();
	if(!linkEnv) return QPointF();

	QPointF basePoint;

	if(type() == CLinkSegment::First)
	{
		basePoint = linkEnv->firstFixedPoint();
	}
	else if(type() == CLinkSegment::Medium)
	{
		basePoint = linkEnv->massCenter();
	}
	else if(type() == CLinkSegment::Second)
	{
		basePoint = linkEnv->secondFixedPoint();
	}

	if(orientation() == CLinkSegment::Horizontal ||
	   orientation() == CLinkSegment::TopHorizontal ||
	   orientation() == CLinkSegment::BottomHorizontal)
		return QPointF(basePoint.x(), managedCoord.shiftedManagedCoord());
	if(orientation() == CLinkSegment::Vertical ||
	   orientation() == CLinkSegment::LeftVertical ||
	   orientation() == CLinkSegment::RightVertical)
		return QPointF(managedCoord.shiftedManagedCoord(), basePoint.y());

	return basePoint;
}

bool CLinkSegment::isHorizontal(void)
{
	return (m_orientation == CLinkSegment::Horizontal ||
			m_orientation == CLinkSegment::TopHorizontal ||
			m_orientation == CLinkSegment::BottomHorizontal);
}

bool CLinkSegment::isVertical(void)
{
	return (m_orientation == CLinkSegment::Vertical ||
			m_orientation == CLinkSegment::LeftVertical ||
			m_orientation == CLinkSegment::RightVertical);
}

QPointF CLinkSegment::center(void)
{
	QRectF segRect = QRectF(m_firstPoint, m_secondPoint);

	return segRect.center();
}

void CLinkSegment::update(void)
{
	m_managedCoord = autoCalc();

	if(isHorizontal())
	{
		m_managedCoord.setManagedOffset(m_managedOffset.y());
	}
	else if(isVertical())
	{
		m_managedCoord.setManagedOffset(m_managedOffset.x());
	}
}

/*!
 * \class CFirstSegment
 */
stManagedCoord CFirstSegment::autoCalc(void)
{
	if(!link()) return stManagedCoord();

	CLinkEnv *linkEnv = link()->linkEnv();
	if(!linkEnv) return stManagedCoord();

	stManagedCoord mc;
	if(orientation() == CLinkSegment::Horizontal)
	{
		mc = stManagedCoord(linkEnv->firstFixedPoint().y(), stManagedCoord::Y);
	}
	else if(orientation() == CLinkSegment::TopHorizontal)
	{
		mc = stManagedCoord(linkEnv->firstFixedFigure().top(), stManagedCoord::Y);
	}
	else if(orientation() == CLinkSegment::BottomHorizontal)
	{
		mc = stManagedCoord(linkEnv->firstFixedFigure().bottom(), stManagedCoord::Y);
	}
	else if(orientation() == CLinkSegment::Vertical)
	{
		mc = stManagedCoord(linkEnv->firstFixedPoint().x(), stManagedCoord::X);
	}
	else if(orientation() == CLinkSegment::LeftVertical)
	{
		mc = stManagedCoord(linkEnv->firstFixedFigure().left(), stManagedCoord::X);
	}
	else if(orientation() == CLinkSegment::RightVertical)
	{
		mc = stManagedCoord(linkEnv->firstFixedFigure().right(), stManagedCoord::X);
	}

	return mc;
}

CFirstSegment::CFirstSegment(CLinkSegments *linkSegments) : CLinkSegment(linkSegments, CLinkSegment::First)
{
}

/*!
 * \class CMedSegment
 */
CLinkSegment::Orientation CMedSegment::invertOrient(const CLinkSegment::Orientation &orientation)
{
	switch((int)orientation)
	{
		case CLinkSegment::BottomHorizontal:
		case CLinkSegment::TopHorizontal:
		case CLinkSegment::Horizontal: return CLinkSegment::Vertical;
		case CLinkSegment::LeftVertical:
		case CLinkSegment::RightVertical:
		case CLinkSegment::Vertical: return CLinkSegment::Horizontal;
	}

	return CLinkSegment::NoOrientation;
}

bool CMedSegment::isParallel(const CLinkSegment::Orientation &firstOrient, const CLinkSegment::Orientation &secondOrient)
{
	if(((firstOrient == CLinkSegment::Horizontal ||
		firstOrient == CLinkSegment::TopHorizontal ||
		firstOrient == CLinkSegment::BottomHorizontal) &&
	   (secondOrient == CLinkSegment::Horizontal ||
		secondOrient == CLinkSegment::TopHorizontal ||
		secondOrient == CLinkSegment::BottomHorizontal)) ||
	   ((firstOrient == CLinkSegment::Vertical ||
		 firstOrient == CLinkSegment::LeftVertical ||
		 firstOrient == CLinkSegment::RightVertical) &&
		(secondOrient == CLinkSegment::Vertical ||
		 secondOrient == CLinkSegment::LeftVertical ||
		 secondOrient == CLinkSegment::RightVertical))) return true;

	return false;
}

stManagedCoord CMedSegment::autoCalc(void)
{
	stManagedCoord mc;
	if(!link()) return mc;

	CLinkSegment *firstSegment = segment(CLinkSegment::First);
	CLinkSegment *secondSegment = segment(CLinkSegment::Second);
	if(!firstSegment || !secondSegment) return mc;

	if(isParallel(firstSegment->orientation(), secondSegment->orientation()))
	{
		setOrientation(invertOrient(firstSegment->orientation()));
	}
	else
	{
		setOrientation(CLinkSegment::NoOrientation);
	}

	CLinkEnv *linkEnv = link()->linkEnv();
	if(!linkEnv) return mc;

	QPointF massCenter = linkEnv->massCenter();

	if(orientation() == CLinkSegment::Horizontal)
	{
		mc = stManagedCoord(massCenter.y(), stManagedCoord::Y);
	}
	else if(orientation() == CLinkSegment::Vertical)
	{
		mc = stManagedCoord(massCenter.x(), stManagedCoord::X);
	}
	else if(orientation() == CLinkSegment::NoOrientation)
	{
		mc.setType(stManagedCoord::XY);
	}
	return mc;
}

CMedSegment::CMedSegment(CLinkSegments *linkSegments) : CLinkSegment(linkSegments, CLinkSegment::Medium)
{
}

/*!
 * \class CSecondSegment
 */
stManagedCoord CSecondSegment::autoCalc(void)
{
	if(!link()) return stManagedCoord();

	CLinkEnv *linkEnv = link()->linkEnv();
	if(!linkEnv) return stManagedCoord();

	stManagedCoord mc;
	if(orientation() == CLinkSegment::Horizontal)
	{
		mc = stManagedCoord(linkEnv->secondFixedPoint().y(), stManagedCoord::Y);
	}
	else if(orientation() == CLinkSegment::TopHorizontal)
	{
		mc = stManagedCoord(linkEnv->secondFixedFigure().top(), stManagedCoord::Y);
	}
	else if(orientation() == CLinkSegment::BottomHorizontal)
	{
		mc = stManagedCoord(linkEnv->secondFixedFigure().bottom(), stManagedCoord::Y);
	}
	else if(orientation() == CLinkSegment::Vertical)
	{
		mc = stManagedCoord(linkEnv->secondFixedPoint().x(), stManagedCoord::X);
	}
	else if(orientation() == CLinkSegment::LeftVertical)
	{
		mc = stManagedCoord(linkEnv->secondFixedFigure().left(), stManagedCoord::X);
	}
	else if(orientation() == CLinkSegment::RightVertical)
	{
		mc = stManagedCoord(linkEnv->secondFixedFigure().right(), stManagedCoord::X);
	}

	return mc;
}

CSecondSegment::CSecondSegment(CLinkSegments *linkSegments) : CLinkSegment(linkSegments, CLinkSegment::Second)
{
}

/*!
 * \class CLinkSegments
 */

void CLinkSegments::calcLeftLeft(const QPointF &ffp, const QRectF &fff, const QPointF &sfp, const QRectF &sff,
								 CLinkSegment::Orientation &firstOrient, CLinkSegment::Orientation &secondOrient)
{
	if(sfp.y() < ffp.y())
	{
		if(sfp.x() < ffp.x())
		{
			if(sff.bottom() < ffp.y())
			{
				firstOrient = CLinkSegment::Horizontal;
				secondOrient = CLinkSegment::Vertical;
			}
			else
			{
				firstOrient = CLinkSegment::Horizontal;
				secondOrient = CLinkSegment::BottomHorizontal;
			}
		}
		else if(sfp.x() >= ffp.x() && sfp.y() < fff.top())
		{
			firstOrient = CLinkSegment::Vertical;
			secondOrient = CLinkSegment::Horizontal;
		}
		else if(sfp.x() >= ffp.x() && sfp.y() >= fff.top() && sfp.y() <= fff.bottom())
		{
			firstOrient = CLinkSegment::TopHorizontal;
			secondOrient = CLinkSegment::Horizontal;
		}
	}
	if(sfp.y() >= ffp.y())
	{
		if(sfp.x() < ffp.x())
		{
			if(sff.top() > ffp.y())
			{
				firstOrient = CLinkSegment::Horizontal;
				secondOrient = CLinkSegment::Vertical;
			}
			else
			{
				firstOrient = CLinkSegment::Horizontal;
				secondOrient = CLinkSegment::TopHorizontal;
			}
		}
		else if(sfp.x() >= ffp.x() && sfp.y() > fff.bottom())
		{
			firstOrient = CLinkSegment::Vertical;
			secondOrient = CLinkSegment::Horizontal;
		}
		else if(sfp.x() >= ffp.x() && sfp.y() >= fff.top() && sfp.y() <= fff.bottom())
		{
			firstOrient = CLinkSegment::BottomHorizontal;
			secondOrient = CLinkSegment::Horizontal;
		}
	}
}

void CLinkSegments::calcLeftTop(const QPointF &ffp, const QRectF &fff, const QPointF &sfp, const QRectF &sff, CLinkSegment::Orientation &firstOrient, CLinkSegment::Orientation &secondOrient)
{
	if(sfp.y() < ffp.y())
	{
		if(sff.right() < ffp.x())
		{
			firstOrient = CLinkSegment::Horizontal;
			secondOrient = CLinkSegment::Horizontal;
		}
		else if(sff.left() < ffp.x())
		{
			firstOrient = CLinkSegment::Horizontal;
			secondOrient = CLinkSegment::LeftVertical;
		}
		else if(sff.left() >= ffp.x() && sfp.y() < fff.top())
		{
			firstOrient = CLinkSegment::Vertical;
			secondOrient = CLinkSegment::Horizontal;
		}
		else if(sff.left() >= ffp.x() && sfp.y() >= fff.top() && sfp.y() <= fff.bottom())
		{
			firstOrient = CLinkSegment::TopHorizontal;
			secondOrient = CLinkSegment::Vertical;
		}
	}
	if(sfp.y() >= ffp.y())
	{
		if(sff.right() < ffp.x())
		{
			firstOrient = CLinkSegment::Horizontal;
			secondOrient = CLinkSegment::Vertical;
		}
		else if(sfp.x() < ffp.x())
		{
			firstOrient = CLinkSegment::Horizontal;
			secondOrient = CLinkSegment::Vertical;
		}
		else if(sfp.x() >= ffp.x() && sfp.y() > fff.bottom())
		{
			firstOrient = CLinkSegment::Vertical;
			secondOrient = CLinkSegment::Vertical;
		}
		else if(sfp.x() >= ffp.x() && sfp.y() >= fff.top() && sfp.y() <= fff.bottom())
		{
			firstOrient = CLinkSegment::BottomHorizontal;
			secondOrient = CLinkSegment::Horizontal;
		}
	}
}

void CLinkSegments::calcLeftRight(const QPointF &ffp, const QRectF &fff, const QPointF &sfp, const QRectF &sff, CLinkSegment::Orientation &firstOrient, CLinkSegment::Orientation &secondOrient)
{
	if(sfp.y() < ffp.y())
	{
		if(sfp.x() < ffp.x())
		{
			firstOrient = CLinkSegment::Horizontal;
			secondOrient = CLinkSegment::Horizontal;
		}
		else if(sfp.x() >= ffp.x() && sff.bottom() < fff.top())
		{
			firstOrient = CLinkSegment::Vertical;
			secondOrient = CLinkSegment::Vertical;
		}
		else if(sfp.x() >= ffp.x() && sff.bottom() >= fff.top())
		{
			firstOrient = CLinkSegment::TopHorizontal;
			secondOrient = CLinkSegment::BottomHorizontal;
		}
	}
	if(sfp.y() >= ffp.y())
	{
		if(sfp.x() < ffp.x())
		{
			firstOrient = CLinkSegment::Horizontal;
			secondOrient = CLinkSegment::Horizontal;
		}
		else if(sfp.x() >= ffp.x() && sff.top() > fff.bottom())
		{
			firstOrient = CLinkSegment::Vertical;
			secondOrient = CLinkSegment::Vertical;
		}
		else if(sfp.x() >= ffp.x() && sff.top() <= fff.bottom())
		{
			firstOrient = CLinkSegment::BottomHorizontal;
			secondOrient = CLinkSegment::TopHorizontal;
		}
	}
}

void CLinkSegments::calcLeftBottom(const QPointF &ffp, const QRectF &fff, const QPointF &sfp, const QRectF &sff, CLinkSegment::Orientation &firstOrient, CLinkSegment::Orientation &secondOrient)
{
	QPointF _ffp = QPointF(ffp.x(), -ffp.y());
	QRectF _fff = QRectF(QPointF(fff.left(), -fff.bottom()), QPointF(fff.right(), -fff.top()));
	QPointF _sfp = QPointF(sfp.x(), -sfp.y());
	QRectF _sff = QRectF(QPointF(sff.left(), -sff.bottom()), QPointF(sff.right(), -sff.top()));

	calcLeftTop(_ffp, _fff, _sfp, _sff, firstOrient, secondOrient);

	switch((int)firstOrient)
	{
		case CLinkSegment::TopHorizontal: firstOrient = CLinkSegment::BottomHorizontal; break;
		case CLinkSegment::BottomHorizontal: firstOrient = CLinkSegment::TopHorizontal; break;
	}
	switch((int)secondOrient)
	{
		case CLinkSegment::TopHorizontal: secondOrient = CLinkSegment::BottomHorizontal; break;
		case CLinkSegment::BottomHorizontal: secondOrient = CLinkSegment::TopHorizontal; break;
	}
}

void CLinkSegments::calcTopTop(const QPointF &ffp, const QRectF &fff, const QPointF &sfp, const QRectF &sff, CLinkSegment::Orientation &firstOrient, CLinkSegment::Orientation &secondOrient)
{
	QPointF _ffp = QPointF(ffp.y(), ffp.x());
	QRectF _fff = QRectF(QPointF(fff.top(), fff.left()), QPointF(fff.bottom(), fff.right()));
	QPointF _sfp = QPointF(sfp.y(), sfp.x());
	QRectF _sff = QRectF(QPointF(sff.top(), sff.left()), QPointF(sff.bottom(), sff.right()));

	calcLeftLeft(_ffp, _fff, _sfp, _sff, firstOrient, secondOrient);

	switch((int)firstOrient)
	{
		case CLinkSegment::Horizontal: firstOrient = CLinkSegment::Vertical; break;
		case CLinkSegment::Vertical: firstOrient = CLinkSegment::Horizontal; break;
		case CLinkSegment::TopHorizontal: firstOrient = CLinkSegment::LeftVertical; break;
		case CLinkSegment::BottomHorizontal: firstOrient = CLinkSegment::RightVertical; break;
		case CLinkSegment::LeftVertical: firstOrient = CLinkSegment::TopHorizontal; break;
		case CLinkSegment::RightVertical: firstOrient = CLinkSegment::BottomHorizontal; break;
	}
	switch((int)secondOrient)
	{
		case CLinkSegment::Horizontal: secondOrient = CLinkSegment::Vertical; break;
		case CLinkSegment::Vertical: secondOrient = CLinkSegment::Horizontal; break;
		case CLinkSegment::TopHorizontal: secondOrient = CLinkSegment::LeftVertical; break;
		case CLinkSegment::BottomHorizontal: secondOrient = CLinkSegment::RightVertical; break;
		case CLinkSegment::LeftVertical: secondOrient = CLinkSegment::TopHorizontal; break;
		case CLinkSegment::RightVertical: secondOrient = CLinkSegment::BottomHorizontal; break;
	}
}

void CLinkSegments::calcTopRight(const QPointF &ffp, const QRectF &fff, const QPointF &sfp, const QRectF &sff, CLinkSegment::Orientation &firstOrient, CLinkSegment::Orientation &secondOrient)
{
	QPointF _ffp = QPointF(-ffp.x(), ffp.y());
	QRectF _fff = QRectF(QPointF(-fff.right(), fff.top()), QPointF(-fff.left(), fff.bottom()));
	QPointF _sfp = QPointF(-sfp.x(), sfp.y());
	QRectF _sff = QRectF(QPointF(-sff.right(), sff.top()), QPointF(-sff.left(), sff.bottom()));

	calcLeftTop(_sfp, _sff, _ffp, _fff, secondOrient, firstOrient);

	switch((int)firstOrient)
	{
		case CLinkSegment::LeftVertical: firstOrient = CLinkSegment::RightVertical; break;
		case CLinkSegment::RightVertical: firstOrient = CLinkSegment::LeftVertical; break;
	}
	switch((int)secondOrient)
	{
		case CLinkSegment::LeftVertical: secondOrient = CLinkSegment::RightVertical; break;
		case CLinkSegment::RightVertical: secondOrient = CLinkSegment::LeftVertical; break;
	}
}

void CLinkSegments::calcTopBottom(const QPointF &ffp, const QRectF &fff, const QPointF &sfp, const QRectF &sff, CLinkSegment::Orientation &firstOrient, CLinkSegment::Orientation &secondOrient)
{
	QPointF _ffp = QPointF(ffp.y(), ffp.x());
	QRectF _fff = QRectF(QPointF(fff.top(), fff.left()), QPointF(fff.bottom(), fff.right()));
	QPointF _sfp = QPointF(sfp.y(), sfp.x());
	QRectF _sff = QRectF(QPointF(sff.top(), sff.left()), QPointF(sff.bottom(), sff.right()));

	calcLeftRight(_ffp, _fff, _sfp, _sff, firstOrient, secondOrient);

	switch((int)firstOrient)
	{
		case CLinkSegment::Horizontal: firstOrient = CLinkSegment::Vertical; break;
		case CLinkSegment::Vertical: firstOrient = CLinkSegment::Horizontal; break;
		case CLinkSegment::TopHorizontal: firstOrient = CLinkSegment::LeftVertical; break;
		case CLinkSegment::BottomHorizontal: firstOrient = CLinkSegment::RightVertical; break;
		case CLinkSegment::LeftVertical: firstOrient = CLinkSegment::TopHorizontal; break;
		case CLinkSegment::RightVertical: firstOrient = CLinkSegment::BottomHorizontal; break;
	}
	switch((int)secondOrient)
	{
		case CLinkSegment::Horizontal: secondOrient = CLinkSegment::Vertical; break;
		case CLinkSegment::Vertical: secondOrient = CLinkSegment::Horizontal; break;
		case CLinkSegment::TopHorizontal: secondOrient = CLinkSegment::LeftVertical; break;
		case CLinkSegment::BottomHorizontal: secondOrient = CLinkSegment::RightVertical; break;
		case CLinkSegment::LeftVertical: secondOrient = CLinkSegment::TopHorizontal; break;
		case CLinkSegment::RightVertical: secondOrient = CLinkSegment::BottomHorizontal; break;
	}
}

void CLinkSegments::calcRightRight(const QPointF &ffp, const QRectF &fff, const QPointF &sfp, const QRectF &sff, CLinkSegment::Orientation &firstOrient, CLinkSegment::Orientation &secondOrient)
{
	QPointF _ffp = QPointF(-ffp.x(), ffp.y());
	QRectF _fff = QRectF(QPointF(-fff.right(), fff.top()), QPointF(-fff.left(), fff.bottom()));
	QPointF _sfp = QPointF(-sfp.x(), sfp.y());
	QRectF _sff = QRectF(QPointF(-sff.right(), sff.top()), QPointF(-sff.left(), sff.bottom()));

	calcLeftLeft(_ffp, _fff, _sfp, _sff, firstOrient, secondOrient);

	switch((int)firstOrient)
	{
		case CLinkSegment::LeftVertical: firstOrient = CLinkSegment::RightVertical; break;
		case CLinkSegment::RightVertical: firstOrient = CLinkSegment::LeftVertical; break;
	}
	switch((int)secondOrient)
	{
		case CLinkSegment::LeftVertical: secondOrient = CLinkSegment::RightVertical; break;
		case CLinkSegment::RightVertical: secondOrient = CLinkSegment::LeftVertical; break;
	}
}

void CLinkSegments::calcRightBottom(const QPointF &ffp, const QRectF &fff, const QPointF &sfp, const QRectF &sff, CLinkSegment::Orientation &firstOrient, CLinkSegment::Orientation &secondOrient)
{
	QPointF _ffp = QPointF(ffp.x(), -ffp.y());
	QRectF _fff = QRectF(QPointF(fff.left(), -fff.bottom()), QPointF(fff.right(), -fff.top()));
	QPointF _sfp = QPointF(sfp.x(), -sfp.y());
	QRectF _sff = QRectF(QPointF(sff.left(), -sff.bottom()), QPointF(sff.right(), -sff.top()));

	calcTopRight(_sfp, _sff, _ffp, _fff, secondOrient, firstOrient);

	switch((int)firstOrient)
	{
		case CLinkSegment::TopHorizontal: firstOrient = CLinkSegment::BottomHorizontal; break;
		case CLinkSegment::BottomHorizontal: firstOrient = CLinkSegment::TopHorizontal; break;
	}
	switch((int)secondOrient)
	{
		case CLinkSegment::TopHorizontal: secondOrient = CLinkSegment::BottomHorizontal; break;
		case CLinkSegment::BottomHorizontal: secondOrient = CLinkSegment::TopHorizontal; break;
	}
}

void CLinkSegments::calcBottomBottom(const QPointF &ffp, const QRectF &fff, const QPointF &sfp, const QRectF &sff, CLinkSegment::Orientation &firstOrient, CLinkSegment::Orientation &secondOrient)
{
	QPointF _ffp = QPointF(ffp.x(), -ffp.y());
	QRectF _fff = QRectF(QPointF(fff.left(), -fff.bottom()), QPointF(fff.right(), -fff.top()));
	QPointF _sfp = QPointF(sfp.x(), -sfp.y());
	QRectF _sff = QRectF(QPointF(sff.left(), -sff.bottom()), QPointF(sff.right(), -sff.top()));

	calcTopTop(_ffp, _fff, _sfp, _sff, firstOrient, secondOrient);
}

void CLinkSegments::calc(void)
{
	foreach(CLinkSegment *segment, m_segments)
	{
		if(segment) segment->setOrientation(CLinkSegment::NoOrientation);
	}

	CLinkSegment *firstSegment = segment(CLinkSegment::First);
	CLinkSegment *medSegment = segment(CLinkSegment::Medium);
	CLinkSegment *secondSegment = segment(CLinkSegment::Second);

	CLinkSegment::Orientation firstOrient;
	CLinkSegment::Orientation secondOrient;

	CLinkEnv *linkEnv = link()->linkEnv();
	if(!linkEnv) return;

	QPointF ffp = linkEnv->firstFixedPoint();
	QRectF fff = linkEnv->firstFixedFigure();
	QPointF sfp = linkEnv->secondFixedPoint();
	QRectF sff = linkEnv->secondFixedFigure();

	switch(linkEnv->firstPortalOrientation())
	{
		case CPortal::Left:
			switch(linkEnv->secondPortalOrientation())
			{
				case CPortal::Left:
					calcLeftLeft(ffp, fff, sfp, sff, firstOrient, secondOrient);
				break;
				case CPortal::Top:
					calcLeftTop(ffp, fff, sfp, sff, firstOrient, secondOrient);
				break;
				case CPortal::Right:
					calcLeftRight(ffp, fff, sfp, sff, firstOrient, secondOrient);
				break;
				case CPortal::Bottom:
					calcLeftBottom(ffp, fff, sfp, sff, firstOrient, secondOrient);
				break;
				case CPortal::NoOrientation: break;
			}
		break;
		case CPortal::Top:
			switch(linkEnv->secondPortalOrientation())
			{
				case CPortal::Left:
					calcLeftTop(sfp, sff, ffp, fff, secondOrient, firstOrient);
				break;
				case CPortal::Top:
					calcTopTop(ffp, fff, sfp, sff, firstOrient, secondOrient);
				break;
				case CPortal::Right:
					calcTopRight(ffp, fff, sfp, sff, firstOrient, secondOrient);
				break;
				case CPortal::Bottom:
					calcTopBottom(ffp, fff, sfp, sff, firstOrient, secondOrient);
				break;
				case CPortal::NoOrientation: break;
			}
		break;
		case CPortal::Right:
			switch(linkEnv->secondPortalOrientation())
			{
				case CPortal::Left:
					calcLeftRight(sfp, sff, ffp, fff, secondOrient, firstOrient);
				break;
				case CPortal::Top:
					calcTopRight(sfp, sff, ffp, fff, secondOrient, firstOrient);
				break;
				case CPortal::Right:
					calcRightRight(ffp, fff, sfp, sff, firstOrient, secondOrient);
				break;
				case CPortal::Bottom:
					calcRightBottom(ffp, fff, sfp, sff, firstOrient, secondOrient);
				break;
				case CPortal::NoOrientation: break;
			}
		break;
		case CPortal::Bottom:
			switch(linkEnv->secondPortalOrientation())
			{
				case CPortal::Left:
					calcLeftBottom(sfp, sff, ffp, fff, secondOrient, firstOrient);
				break;
				case CPortal::Top:
					calcTopBottom(sfp, sff, ffp, fff, secondOrient, firstOrient);
				break;
				case CPortal::Right:
					calcRightBottom(sfp, sff, ffp, fff, secondOrient, firstOrient);
				break;
				case CPortal::Bottom:
					calcBottomBottom(ffp, fff, sfp, sff, firstOrient, secondOrient);
				break;
				case CPortal::NoOrientation: break;
			}
		break;
		case CPortal::NoOrientation: break;
	}

	if(firstSegment)
	{
		firstSegment->setOrientation(firstOrient);
		firstSegment->update();
	}
	if(secondSegment)
	{
		secondSegment->setOrientation(secondOrient);
		secondSegment->update();
	}
	if(medSegment) medSegment->update();

	QPointF fs1, fs2, ms1, ms2, ss1, ss2;

	fs1 = firstSegment->managedToPoint(firstSegment->managedCoord());

	stManagedCoord medMC = medSegment->managedCoord();
	if(medMC.type() == stManagedCoord::XY)
	{
		QPointF crossPoint;
		if(firstSegment->isHorizontal() && secondSegment->isVertical())
		{
			crossPoint = QPointF(secondSegment->managedCoord().shiftedManagedCoord(), firstSegment->managedCoord().shiftedManagedCoord());
		}
		else if(secondSegment->isHorizontal() && firstSegment->isVertical())
		{
			crossPoint = QPointF(firstSegment->managedCoord().shiftedManagedCoord(), secondSegment->managedCoord().shiftedManagedCoord());
		}

		ms1 = ms2 = crossPoint;
	}
	else if(medMC.type() == stManagedCoord::X)
	{
		ms1 = QPointF(medMC.shiftedManagedCoord(), firstSegment->managedCoord().shiftedManagedCoord());
		ms2 = QPointF(medMC.shiftedManagedCoord(), secondSegment->managedCoord().shiftedManagedCoord());
	}
	else if(medMC.type() == stManagedCoord::Y)
	{
		ms1 = QPointF(firstSegment->managedCoord().shiftedManagedCoord(), medMC.shiftedManagedCoord());
		ms2 = QPointF(secondSegment->managedCoord().shiftedManagedCoord(), medMC.shiftedManagedCoord());
	}

	fs2 = ms1;
	ss1 = ms2;

	ss2 = secondSegment->managedToPoint(secondSegment->managedCoord());

	firstSegment->setCoordinates(fs1, fs2);
	medSegment->setCoordinates(ms1, ms2);
	secondSegment->setCoordinates(ss1, ss2);
}

stManagedCoord CLinkSegments::segmentManagedCoord(const CLinkSegment::Type &type)
{
	CLinkSegment *seg = segment(type);
	if(!seg) return stManagedCoord();

	return seg->managedCoord();
}

QPointF CLinkSegments::managedCoordToPoint(const stManagedCoord &managedCoord, const QPointF &basePoint)
{
	if(!managedCoord.isValid()) return QPointF();

	if(managedCoord.type() == stManagedCoord::X) return QPointF(managedCoord.shiftedManagedCoord(), basePoint.y());
	if(managedCoord.type() == stManagedCoord::Y) return QPointF(basePoint.x(), managedCoord.shiftedManagedCoord());
	if(managedCoord.type() == stManagedCoord::XY) return basePoint;

	return basePoint;
}

CLinkSegments::CLinkSegments(CLink *link)
{
	m_link = 0;

	m_segments[CLinkSegment::First] = new CFirstSegment(this);
	m_segments[CLinkSegment::Medium] = new CMedSegment(this);
	m_segments[CLinkSegment::Second] = new CSecondSegment(this);

	setLink(link);
}

CLinkSegments::~CLinkSegments(void)
{
	foreach(CLinkSegment *segment, m_segments)
	{
		if(segment) delete segment;
	}
}

void CLinkSegments::setLink(CLink *link)
{
	if(m_link && (m_link == link)) return;

	m_link = link;

	calc();
}

CLinkSegment* CLinkSegments::segment(const CLinkSegment::Type &type)
{
	if(!m_segments.contains(type)) return 0;

	return m_segments[type];
}

QList<QPointF> CLinkSegments::points(void)
{
	calc();

	QList<QPointF> _points;

	CLinkSegment *firstSegment = segment(CLinkSegment::First);
	CLinkSegment *medSegment = segment(CLinkSegment::Medium);
	CLinkSegment *secondSegment = segment(CLinkSegment::Second);
	if(!firstSegment || !medSegment || !secondSegment) return _points;

	_points << firstSegment->firstPoint();
	_points << medSegment->firstPoint();
	if(medSegment->secondPoint() != medSegment->firstPoint()) _points << medSegment->secondPoint();
	_points << secondSegment->secondPoint();

	return _points;
}