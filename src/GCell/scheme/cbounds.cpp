#include "cbounds.h"

#include <QPainter>

/*!
 * \class CBounds
 */
CBounds::CBounds(const QRectF &bounds)
{
	setBounds(bounds);
}

bool CBounds::isOutBounds(const QPointF &pos, const QRectF &itemBoundingRect)
{
	return !m_bounds.contains(QRectF(pos, itemBoundingRect.size()));
}

QPointF CBounds::posByBound(const QPointF &newPos, const QRectF &itemBoundingRect)
{
	QPointF resPos = newPos;
	if(newPos.x() < bounds().left()) resPos.setX(bounds().left());
	if(newPos.y() < bounds().top()) resPos.setY(bounds().top());
	if(newPos.x() + itemBoundingRect.width() > bounds().right()) resPos.setX(bounds().right() - itemBoundingRect.width());
	if(newPos.y() + itemBoundingRect.height() > bounds().bottom()) resPos.setY(bounds().bottom() - itemBoundingRect.height());
	return resPos;
}

void CBounds::paint(QPainter *painter)
{
	if(!painter) return;
	painter->save();
	painter->drawRect(bounds());
	painter->restore();
}