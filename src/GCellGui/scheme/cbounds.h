#ifndef CBOUNDS_H
#define CBOUNDS_H

#include <QRectF>

class QPainter;

class CBounds
{
private:
	QRectF m_bounds;
public:
	CBounds(const QRectF &bounds = QRectF());

	const QRectF& bounds(void) const{return m_bounds;}
	void setBounds(const QRectF &bounds){m_bounds = bounds;}

	bool inBounds(const QPointF &pos) const{return m_bounds.contains(pos);}
	bool isOutBounds(const QPointF &pos, const QRectF &itemBoundingRect);
	QPointF posByBound(const QPointF &newPos, const QRectF &itemBoundingRect);
	void paint(QPainter *painter);
};

#endif // CBOUNDS_H