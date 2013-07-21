#include "clink.h"

#include <QPainter>

#include "cportal.h"

void CLink::calc(void)
{
	if(!m_firstPortal || !m_secondPortal)
	{
		m_path = QPainterPath();
		return;
	}

	m_path.moveTo(m_firstPortal->mapToScene(m_firstPortal->pos()));
	m_path.lineTo(m_secondPortal->mapToScene(m_secondPortal->pos()));
}

CLink::CLink(QGraphicsItem *parent) : CElement(parent)
{
    setObjectName(QStringLiteral("CLink"));

	m_firstPortal = 0;
	m_secondPortal = 0;

	setDefaultName(tr("link"));
}

QPainterPath CLink::shape(void) const
{
	return m_path;
}

QRectF CLink::boundingRect(void) const
{
	return m_path.controlPointRect();
}

void CLink::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)

	painter->save();
	painter->drawPath(m_path);
	painter->restore();
}

void CLink::updateGeometry(void)
{
	prepareGeometryChange();
	calc();
	update();
}