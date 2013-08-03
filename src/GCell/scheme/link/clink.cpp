#include "clink.h"

#include <QPainter>

#include "../portal/cresult.h"
#include "../portal/cargument.h"
#include "../cscheme.h"

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

	QPointF p0 = m_result->mapToScene(m_result->linkPos());
	QPointF p1 = p0;
	QPointF p3 = m_argument->mapToScene(m_argument->linkPos());
	QPointF p2 = p3;

	switch((int)m_result->portalOrientation())
	{
		case CPortal::Left: p1.setX(p1.x() - m_distFromPortal); break;
		case CPortal::Top: p1.setY(p1.y() - m_distFromPortal); break;
		case CPortal::Right: p1.setX(p1.x() + m_distFromPortal); break;
		case CPortal::Bottom: p1.setY(p1.y() + m_distFromPortal); break;
	}
	switch((int)m_argument->portalOrientation())
	{
		case CPortal::Left: p2.setX(p2.x() - m_distFromPortal); break;
		case CPortal::Top: p2.setY(p2.y() - m_distFromPortal); break;
		case CPortal::Right: p2.setX(p2.x() + m_distFromPortal); break;
		case CPortal::Bottom: p2.setY(p2.y() + m_distFromPortal); break;
	}

	m_path.moveTo(p0);
	m_path.lineTo(p1);
	m_path.lineTo(p2);
	m_path.lineTo(p3);
}

CLink::CLink(QGraphicsItem *parent) : CElement(parent)
{
    setObjectName(QStringLiteral("CLink"));

	m_result = 0;
	m_argument = 0;
	m_distFromPortal = 10.0;

	setDefaultName(tr("link"));

	setFlag(QGraphicsItem::ItemIsSelectable);
}

CLink::~CLink(void)
{
	if(m_result) m_result->removeLink(this);
	if(m_argument)
	{
		m_argument->setBuffer(0);
		m_argument->removeLink(this);
	}
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

	QPen pen;
	if(isSelected())
	{
		pen.setWidthF(2.0);
	}

	painter->save();
	painter->setPen(pen);
	painter->setRenderHint(QPainter::Antialiasing);
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
	setArgument(qobject_cast<CArgument*>(scheme()->element(argumentID)));
}

QString CLink::resultID(void)
{
	if(m_result) return m_result->id();
	return QString();
}

void CLink::setResultID(const QString &resultID)
{
	setResult(qobject_cast<CResult*>(scheme()->element(resultID)));
}

void CLink::setResult(CResult *result)
{
	if(m_result && (m_result == result)) return;
	if(m_result)
	{
		disconnect(m_result, SIGNAL(destroyed(QObject*)), this, SLOT(onResultDestroyed(QObject*)));
		m_result->removeLink(this);
		if(m_argument) m_argument->setBuffer(0);
	}
	m_result = result;
	if(m_result)
	{
		connect(m_result, SIGNAL(destroyed(QObject*)), this, SLOT(onResultDestroyed(QObject*)));
		m_result->addLink(this);
		if(m_argument) m_argument->setBuffer(result->buffer());
	}
	updateGeometry();
}

void CLink::setArgument(CArgument *argument)
{
	if(m_argument && (m_argument == argument)) return;
	if(m_argument)
	{
		disconnect(m_argument, SIGNAL(destroyed(QObject*)), this, SLOT(onArgumentDestroyed(QObject*)));
		m_argument->removeLink(this);
		if(m_argument) m_argument->setBuffer(0);
	}
	m_argument = argument;
	if(m_argument)
	{
		connect(m_argument, SIGNAL(destroyed(QObject*)), this, SLOT(onArgumentDestroyed(QObject*)));
		m_argument->addLink(this);
		if(m_result) m_argument->setBuffer(m_result->buffer());
	}
	updateGeometry();
}

void CLink::calc(const int &timeFrame)
{
	if(m_argument) m_argument->calc(timeFrame);
}

void CLink::onResultDestroyed(QObject *objResult)
{
	if(m_result == (CResult*)objResult) m_result = 0;
	deleteLater();
}

void CLink::onArgumentDestroyed(QObject *objArgument)
{
	if(m_argument == (CArgument*)objArgument) m_argument = 0;
	deleteLater();
}

void CLink::updateGeometry(void)
{
	prepareGeometryChange();
	calcLink();
	update();
}