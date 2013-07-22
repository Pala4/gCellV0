#include "clink.h"

#include <QPainter>

#include "cportal.h"

QPainterPath CLink::shapeFromPath(const QPainterPath &path, const QPen &pen)
{
    // We unfortunately need this hack as QPainterPathStroker will set a width of 1.0
    // if we pass a value of 0.0 to QPainterPathStroker::setWidth()
    const qreal penWidthZero = qreal(0.00000001);

    if(path == QPainterPath()) return path;
    QPainterPathStroker ps;
    ps.setCapStyle(pen.capStyle());
    if(pen.widthF() <= 0.0)
    {
        ps.setWidth(penWidthZero);
    }
    else
    {
        ps.setWidth(pen.widthF());
    }
    ps.setJoinStyle(pen.joinStyle());
    ps.setMiterLimit(pen.miterLimit());
    QPainterPath p = ps.createStroke(path);
    p.addPath(path);
    return p;
}

void CLink::calc(void)
{
    m_path = QPainterPath();
    if(!m_firstPortal || !m_secondPortal) return;
    m_path.moveTo(m_firstPortal->mapToScene(QPointF()));
    m_path.lineTo(m_secondPortal->mapToScene(QPointF()));
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
    return shapeFromPath(m_path, QPen());
}

QRectF CLink::boundingRect(void) const
{
    return shape().controlPointRect();
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