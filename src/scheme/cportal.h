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

	QList<CLink*> m_links;
protected:
	virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);
public:
	explicit CPortal(QGraphicsItem *parent = 0);

	virtual QRectF boundingRect(void) const;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	const CPortal::TPortalOrientation& portalOrientation(void) const{return m_portalOrientation;}
	void setPortalOrientation(const CPortal::TPortalOrientation &portalOrientation){m_portalOrientation = portalOrientation;}

	void addLink(CLink *link);
};

#endif // CPORTAL_H