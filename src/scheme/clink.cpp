#include "clink.h"

#include <QPainter>

#include "cportal.h"
#include "cscheme.h"

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

void CLink::calcLink(void)
{
    m_path = QPainterPath();
	if(!m_result || !m_argument) return;
	m_path.moveTo(m_result->mapToScene(QPointF()));
	m_path.lineTo(m_argument->mapToScene(QPointF()));
}

CLink::CLink(QGraphicsItem *parent) : CElement(parent)
{
    setObjectName(QStringLiteral("CLink"));

	m_result = 0;
	m_argument = 0;

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

QString CLink::argumentID(void)
{
	if(m_argument) return m_argument->id();
	return QString();
}

void CLink::setArgumentID(const QString &argumentID)
{
	m_argument = qobject_cast<CPortal*>(scheme()->element(argumentID));
	if(m_argument) m_argument->addLink(this);
	updateGeometry();
}

QString CLink::resultID(void)
{
	if(m_result) return m_result->id();
	return QString();
}

void CLink::setResultID(const QString &resultID)
{
	m_result = qobject_cast<CPortal*>(scheme()->element(resultID));
	m_result->addLink(this);
	updateGeometry();
}

void CLink::calc(const int &timeFrame)
{
	if(m_argument)
	{
		if(m_result) m_argument->setPortalData(timeFrame, m_result->portalData(timeFrame));
		m_argument->calc(timeFrame);
	}
}

void CLink::updateGeometry(void)
{
	prepareGeometryChange();
	calcLink();
	update();
}