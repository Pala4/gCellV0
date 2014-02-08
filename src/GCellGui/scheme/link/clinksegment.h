#ifndef CLINKSEGMENT_H
#define CLINKSEGMENT_H

#include <QMap>
#include <QPointF>
#include <QString>

#include "clink.h"
#include "../portal/cportal.h"

class CLinkSegments;

struct stManagedCoord
{
public:
	enum Type{NoType, X, Y, XY};
private:
	qreal m_managedCoord;
	qreal m_managedOffset;
	stManagedCoord::Type m_type;
public:
	stManagedCoord(void) : m_managedCoord(0.0), m_managedOffset(0.0), m_type(stManagedCoord::NoType){}
	stManagedCoord(const qreal &managedCoord, const stManagedCoord::Type &type):
		m_managedCoord(managedCoord), m_managedOffset(0.0), m_type(type){}

	qreal shiftedManagedCoord(void) const{return m_managedCoord + m_managedOffset;}
	const qreal& managedCoord(void) const{return m_managedCoord;}
	void setManagedCoord(const qreal managedCoord){m_managedCoord = managedCoord;}
	const qreal& managedOffset(void) const{return m_managedOffset;}
	void setManagedOffset(const qreal &managedOffset){m_managedOffset = managedOffset;}
	const stManagedCoord::Type& type(void) const{return m_type;}
	void setType(const stManagedCoord::Type &type){m_type = type;}

	bool isValid(void) const{return (m_type != stManagedCoord::NoType);}
};

class CLinkSegment
{
public:
	enum Type{NoType, First, Medium, Second};
	enum Orientation{NoOrientation,
					 Horizontal,
					 Vertical,
					 TopHorizontal,
					 BottomHorizontal,
					 LeftVertical,
					 RightVertical};
private:
	CLinkSegments *m_linkSegments;
	CLinkSegment::Type m_type;
	CLinkSegment::Orientation m_orientation;

	stManagedCoord m_managedCoord;
	QPointF m_managedOffset;

	QPointF m_firstPoint;
	QPointF m_secondPoint;
protected:
	CLink* link(void);
	CLinkSegment* segment(const CLinkSegment::Type &type);
	CLinkSegment::Orientation orientByPortal(const CPortal::TPortalOrientation &portalOrientation);
	CLinkSegment::Orientation invertOrient(const CLinkSegment::Orientation &orientation);

	virtual stManagedCoord autoCalc(void) = 0;
public:
	explicit CLinkSegment(CLinkSegments *linkSegments, const CLinkSegment::Type &type);
	virtual ~CLinkSegment(void){}

	CLinkSegments* linkSegments(void){return m_linkSegments;}
	void setLinkSegments(CLinkSegments *linkSegments);

	const CLinkSegment::Type& type(void) const{return m_type;}
	void setType(const CLinkSegment::Type &type){m_type = type;}

	const CLinkSegment::Orientation& orientation(void) const{return m_orientation;}
	void setOrientation(const CLinkSegment::Orientation &orientation){m_orientation = orientation;}

	const stManagedCoord& managedCoord(void) const{return m_managedCoord;}
	QPointF managedToPoint(const stManagedCoord &managedCoord);

	const QPointF& managedOffset(void) const{return m_managedOffset;}
	void setManagedOffset(const QPointF &managedOffset){setManagedXOffset(managedOffset.x()); setManagedYOffset(managedOffset.y());}
	void setManagedXOffset(const qreal &managedXOffset){m_managedOffset.setX(managedXOffset);}
	void setManagedYOffset(const qreal &managedYOffset){m_managedOffset.setY(managedYOffset);}

	bool isHorizontal(void);
	bool isVertical(void);

	void setCoordinates(const QPointF &firstPoint, const QPointF &secondPoint){m_firstPoint = firstPoint, m_secondPoint = secondPoint;}
	const QPointF& firstPoint(void) const{return m_firstPoint;}
	const QPointF& secondPoint(void) const{return m_secondPoint;}
	QPointF center(void);

	void update(void);
};

class CFirstSegment : public CLinkSegment
{
protected:
	virtual stManagedCoord autoCalc(void);
public:
	explicit CFirstSegment(CLinkSegments *linkSegments);
};

class CMedSegment : public CLinkSegment
{
private:
	CLinkSegment::Orientation invertOrient(const CLinkSegment::Orientation &orientation);
	bool isParallel(const CLinkSegment::Orientation &firstOrient, const CLinkSegment::Orientation &secondOrient);
protected:
	virtual stManagedCoord autoCalc(void);
public:
	explicit CMedSegment(CLinkSegments *linkSegments);
};

class CSecondSegment : public CLinkSegment
{
protected:
	virtual stManagedCoord autoCalc(void);
public:
	explicit CSecondSegment(CLinkSegments *linkSegments);
};

class CLinkSegments
{
private:
	CLink *m_link;
	QMap<CLinkSegment::Type, CLinkSegment*> m_segments;

	void calcLeftLeft(const QPointF &ffp, const QRectF &fff, const QPointF &sfp, const QRectF &sff,
					  CLinkSegment::Orientation &firstOrient, CLinkSegment::Orientation &secondOrient);
	void calcLeftTop(const QPointF &ffp, const QRectF &fff, const QPointF &sfp, const QRectF &sff,
					 CLinkSegment::Orientation &firstOrient, CLinkSegment::Orientation &secondOrient);
	void calcLeftRight(const QPointF &ffp, const QRectF &fff, const QPointF &sfp, const QRectF &sff,
					   CLinkSegment::Orientation &firstOrient, CLinkSegment::Orientation &secondOrient);
	void calcLeftBottom(const QPointF &ffp, const QRectF &fff, const QPointF &sfp, const QRectF &sff,
						CLinkSegment::Orientation &firstOrient, CLinkSegment::Orientation &secondOrient);
	void calcTopTop(const QPointF &ffp, const QRectF &fff, const QPointF &sfp, const QRectF &sff,
					CLinkSegment::Orientation &firstOrient, CLinkSegment::Orientation &secondOrient);
	void calcTopRight(const QPointF &ffp, const QRectF &fff, const QPointF &sfp, const QRectF &sff,
					  CLinkSegment::Orientation &firstOrient, CLinkSegment::Orientation &secondOrient);
	void calcTopBottom(const QPointF &ffp, const QRectF &fff, const QPointF &sfp, const QRectF &sff,
					   CLinkSegment::Orientation &firstOrient, CLinkSegment::Orientation &secondOrient);
	void calcRightRight(const QPointF &ffp, const QRectF &fff, const QPointF &sfp, const QRectF &sff,
						CLinkSegment::Orientation &firstOrient, CLinkSegment::Orientation &secondOrient);
	void calcRightBottom(const QPointF &ffp, const QRectF &fff, const QPointF &sfp, const QRectF &sff,
						 CLinkSegment::Orientation &firstOrient, CLinkSegment::Orientation &secondOrient);
	void calcBottomBottom(const QPointF &ffp, const QRectF &fff, const QPointF &sfp, const QRectF &sff,
						  CLinkSegment::Orientation &firstOrient, CLinkSegment::Orientation &secondOrient);
	void calc(void);
	stManagedCoord segmentManagedCoord(const CLinkSegment::Type &type);
	QPointF managedCoordToPoint(const stManagedCoord &managedCoord, const QPointF &basePoint);
public:
	explicit CLinkSegments(CLink *link);
	virtual ~CLinkSegments(void);

	CLink* link(void){return m_link;}
	void setLink(CLink *link);

	CLinkSegment* segment(const CLinkSegment::Type &type);
	QList<CLinkSegment*> segmentList(void){return m_segments.values();}

	QList<QPointF> points(void);
};

#endif // CLINKSEGMENT_H