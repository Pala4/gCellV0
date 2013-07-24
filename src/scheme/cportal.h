#ifndef CPORTAL_H
#define CPORTAL_H

#include <QGraphicsItem>

#include "celement.h"

class CLink;

class CPortal : public CElement
{
	Q_OBJECT
public:
	enum TPortalOrientation{Left, Top, Right, Bottom};
private:
	CPortal::TPortalOrientation m_portalOrientation;
	bool m_loopBackPortal;

	int m_timeFrame;
	qreal m_data;

	QList<CLink*> m_links;
protected:
	virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);
public:
	explicit CPortal(QGraphicsItem *parent = 0);

	virtual QRectF boundingRect(void) const;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	const CPortal::TPortalOrientation& portalOrientation(void) const{return m_portalOrientation;}
	void setPortalOrientation(const CPortal::TPortalOrientation &portalOrientation){m_portalOrientation = portalOrientation;}
	const bool& isLoopBackPortal(void) const{return m_loopBackPortal;}
	void setLoopBackPortal(const bool &loopBackPortal){if(m_loopBackPortal == loopBackPortal) return; m_loopBackPortal = loopBackPortal; update();}

	void setPortalData(const int &timeFrame, const qreal &data){m_timeFrame = timeFrame; m_data = data;}
	bool isDataReady(const int &timeFrame){return (m_timeFrame == timeFrame);}
    qreal portalData(const int &timeFrame);

	const QList<CLink*>& links(void) const{return m_links;}
	void addLink(CLink *link);
};

#endif // CPORTAL_H