#include "cgrid.h"

#include <QPainter>
#include <QtMath>

#include "cbounds.h"

/*!
 * \class CGreed
 */
void CGrid::initBrush(void)
{
	QPen pen;
	pen.setWidthF(m_pointSize);
	pen.setColor(m_color);

	QPixmap pixmap(m_step, m_step);
	pixmap.fill(m_bkGndColor);

	QPainter bkGndPainter(&pixmap);
	bkGndPainter.setPen(pen);
	bkGndPainter.drawPoint(0.0, 0.0);

	m_bkGndBrush = QBrush(pixmap);
}

CGrid::CGrid(const QColor &color, const QColor &bkGndColor, const int &step, const qreal &pointSize, const bool &align, CBounds *bounds)
{
	m_color = color;
	m_bkGndColor = bkGndColor;
	m_step = step;
	m_pointSize = pointSize;
	m_align = align;
	m_bounds = 0;

	setBounds(bounds);
	initBrush();
}

QPointF CGrid::align(const QPointF &pos)
{
	if(m_step == 0 || !isAlign()) return pos;

	qreal numStepsX = pos.x()/qreal(m_step);
	qreal modNumStepsX = qFabs(numStepsX - (int)numStepsX);
	qreal alX = (modNumStepsX > 0.5) ? (qreal)qCeil(numStepsX)*m_step : (qreal)qFloor(numStepsX)*m_step;

	qreal numStepsY = pos.y()/qreal(m_step);
	qreal modNumStepsY = qFabs(numStepsY - (int)numStepsY);
	qreal alY = (modNumStepsY > 0.5) ? (qreal)qCeil(numStepsY)*m_step : (qreal)qFloor(numStepsY)*m_step;

	return QPointF(alX, alY);
}

void CGrid::paint(QPainter *painter)
{
	if(!painter) return;

	painter->save();
	painter->fillRect(m_bounds->bounds(), m_bkGndBrush);
	painter->restore();
}