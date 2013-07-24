#include "cportal.h"

#include <QPainter>

#include "clink.h"

QVariant CPortal::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
	switch((int)change)
	{
		case QGraphicsItem::ItemScenePositionHasChanged:
			foreach(CLink *link, m_links) if(link) link->updateGeometry();
		break;
	}

	return CElement::itemChange(change, value);
}

CPortal::CPortal(QGraphicsItem *parent) : CElement(parent)
{
	setObjectName(QStringLiteral("CPortal"));

	m_portalOrientation = CPortal::Left;
	m_loopBackPortal = false;
	m_timeFrame = -1;
	m_data = 0.0;

	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

QRectF CPortal::boundingRect(void) const
{
	return QRectF(0.0, 0.0, 5.0, 5.0);
}

void CPortal::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)

	QBrush brush;
	brush.setStyle(Qt::SolidPattern);
	if(isLoopBackPortal())
	{
		brush.setColor(Qt::blue);
	}
	else
	{
		brush.setColor(Qt::red);
	}

	painter->save();
	painter->setBrush(brush);
	painter->drawRect(boundingRect());
	painter->restore();
}

qreal CPortal::portalData(const int &timeFrame)
{
	if(isLoopBackPortal() || (m_timeFrame == timeFrame)) return m_data;
	return 0.0;
}

void CPortal::addLink(CLink *link)
{
	if(!link) return;
	if(m_links.contains(link)) return;

	m_links << link;
}