#include "clink.h"

#include <QPainter>

#include "clinkenv.h"
#include "clinksegment.h"
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
	QList<QPointF> pointsList;
	if(linkEnv())
	{
		pointsList << linkEnv()->firstPoint();
		pointsList << linkEnv()->firstFixedPoint();
	}
	if(m_linkSegments) pointsList << m_linkSegments->points();
	if(linkEnv())
	{
		pointsList << linkEnv()->secondFixedPoint();
		pointsList << linkEnv()->secondPoint();
	}
	if(pointsList.count() >= 2)
	{
		for(int ci = 0; ci < pointsList.count() - 1; ++ci)
		{
			m_path.moveTo(pointsList.at(ci));
			m_path.lineTo(pointsList.at(ci + 1));
		}
		if(isCaptionVisible() && m_linkSegments)
		{
			QPointF captionPoint = m_linkSegments->segment(CLinkSegment::Medium)->center();
			m_path.addText(captionPoint, captionFont(), caption());
		}
	}
	emit formChanged();
}

QRectF CLink::calcBounds(void)
{
	 return shape().controlPointRect();
}

CLink::CLink(QGraphicsItem *parent) : CElement(parent)
{
	 setObjectName(QStringLiteral("CLink"));

	 m_result = 0;
	 m_argument = 0;
	 m_distFromPortal = 10.0;
	 m_linkEnv = 0;
	 m_linkSegments = 0;

	 m_linkEnv = new CLinkEnv(this);
	 m_linkSegments = new CLinkSegments(this);

	 setDefaultName(tr("link"));
	 setCaptionVisible(false);
}

CLink::~CLink(void)
{
	 if(m_result) m_result->removeLink(this);
	 if(m_argument)
	 {
		  m_argument->setBuffer(0);
		  m_argument->setDataColor(QColor(255, 255, 255, 0));
		  m_argument->removeLink(this);
	 }
}

void CLink::setDistFromPortal(const qreal &distFromPortal)
{
	if(m_distFromPortal == distFromPortal) return;

	m_distFromPortal = distFromPortal;
	updateGeometry();
}

QPointF CLink::firstPos(void)
{
	return m_result ? m_result->mapToScene(m_result->linkPos()) : QPointF();
}

CPortal::TPortalOrientation CLink::firstOrientation(void)
{
	return m_result ? m_result->portalOrientation() : CPortal::NoOrientation;
}

QRectF CLink::firstFigure(void)
{
	QRectF ff;
	if(m_result && m_result->hostElement())
	{
		ff = QRectF(m_result->hostElement()->pos(), m_result->hostElement()->boundingRect().size());
	}
	return ff;
}

QPointF CLink::secondPos(void)
{
	return m_argument ? m_argument->mapToScene(m_argument->linkPos()) : QPointF();
}

CPortal::TPortalOrientation CLink::secondOrientation(void)
{
	return m_argument ? m_argument->portalOrientation() : CPortal::NoOrientation;
}

QRectF CLink::secondFigure(void)
{
	QRectF sf;
	if(m_argument && m_argument->hostElement())
	{
		sf = QRectF(m_argument->hostElement()->pos(), m_argument->hostElement()->boundingRect().size());
	}
	return sf;
}

QPainterPath CLink::shape(void) const
{
	 return shapeFromPath(m_path, QPen());
}

void CLink::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	 CElement::paint(painter, option, widget);

	 QPen pen;
	 if(isSelected())
	 {
		  pen.setWidthF(2.0);
	 }
	 if(m_result) pen.setColor(m_result->dataColor());

	 painter->save();
	 painter->setPen(pen);
	 painter->setRenderHint(QPainter::Antialiasing);
	 painter->drawPath(m_path);
	 painter->restore();
}

QString CLink::resultID(void)
{
	 if(m_result) return m_result->id();
	 return m_resultID;
}

void CLink::setResultID(const QString &resultID)
{
	 if(scheme())
	 {
		  setResult(qobject_cast<CResult*>(scheme()->element(resultID)));
	 }
	 else
	 {
		  m_resultID = resultID;
	 }
}

QString CLink::argumentID(void)
{
	 if(m_argument) return m_argument->id();
	 return m_argumentID;
}

void CLink::setArgumentID(const QString &argumentID)
{
	 if(scheme())
	 {
		  setArgument(qobject_cast<CArgument*>(scheme()->element(argumentID)));
	 }
	 else
	 {
		  m_argumentID = argumentID;
	 }
}

const QPointF CLink::firstOffset(void) const
{
	if(!m_linkSegments) return QPointF();

	CLinkSegment *ls = m_linkSegments->segment(CLinkSegment::First);
	if(!ls) return QPointF();

	return ls->managedOffset();
}

void CLink::setFirstOffset(const QPointF &firstOffset)
{
	if(!m_linkSegments) return;

	CLinkSegment *ls = m_linkSegments->segment(CLinkSegment::First);
	if(ls) ls->setManagedOffset(firstOffset);
}

const QPointF CLink::medOffset(void) const
{
	if(!m_linkSegments) return QPointF();

	CLinkSegment *ls = m_linkSegments->segment(CLinkSegment::Medium);
	if(!ls) return QPointF();

	return ls->managedOffset();
}

void CLink::setMedOffset(const QPointF &medOffset)
{
	if(!m_linkSegments) return;

	CLinkSegment *ls = m_linkSegments->segment(CLinkSegment::Medium);
	if(ls) ls->setManagedOffset(medOffset);
}

const QPointF CLink::secondOffset(void) const
{
	if(!m_linkSegments) return QPointF();

	CLinkSegment *ls = m_linkSegments->segment(CLinkSegment::Second);
	if(!ls) return QPointF();

	return ls->managedOffset();
}

void CLink::setSecondOffset(const QPointF &secondOffset)
{
	if(!m_linkSegments) return;

	CLinkSegment *ls = m_linkSegments->segment(CLinkSegment::Second);
	if(ls) ls->setManagedOffset(secondOffset);
}

void CLink::setResult(CResult *result)
{
	 if(m_result && (m_result == result)) return;
	 if(m_result)
	 {
		  disconnect(m_result, SIGNAL(dataColorChanged(QColor)), this, SLOT(onResultDataColorChanged(QColor)));
		  disconnect(m_result, SIGNAL(destroyed(QObject*)), this, SLOT(onResultDestroyed(QObject*)));
		  m_result->removeLink(this);
		  if(m_argument)
		  {
				m_argument->setBuffer(0);
				m_argument->setDataColor(QColor());
		  }
	 }
	 m_result = result;
	 if(m_result)
	 {
		  connect(m_result, SIGNAL(dataColorChanged(QColor)), this, SLOT(onResultDataColorChanged(QColor)));
		  connect(m_result, SIGNAL(destroyed(QObject*)), this, SLOT(onResultDestroyed(QObject*)));
		  m_result->addLink(this);
		  if(m_argument)
		  {
				m_argument->setBuffer(result->buffer());
				m_argument->setDataColor(result->dataColor());
		  }
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
		  if(m_argument)
		  {
				m_argument->setBuffer(0);
				m_argument->setDataColor(QColor());
		  }
	 }
	 m_argument = argument;
	 if(m_argument)
	 {
		  connect(m_argument, SIGNAL(destroyed(QObject*)), this, SLOT(onArgumentDestroyed(QObject*)));
		  m_argument->addLink(this);
		  if(m_result)
		  {
				m_argument->setBuffer(m_result->buffer());
				m_argument->setDataColor(m_result->dataColor());
		  }
	 }
	 updateGeometry();
}

QList<CPortal*> CLink::portals(void)
{
	QList<CPortal*> prts;

	if(dynamic_cast<CPortal*>(m_argument)) prts << dynamic_cast<CPortal*>(m_argument);
	if(dynamic_cast<CPortal*>(m_result)) prts << dynamic_cast<CPortal*>(m_result);

	return prts;
}

bool CLink::reIndexing(const QList<CElement*> &elements)
{
	 if(!m_result || !m_argument)
	 {
		  foreach(CElement *element, elements)
		  {
				if(m_result && m_argument) return true;
				if(!element) continue;
				if(!m_result && (element->id() == m_resultID))
				{
					 setResult(qobject_cast<CResult*>(element));
				}
				if(!m_argument && (element->id() == m_argumentID))
				{
					 setArgument(qobject_cast<CArgument*>(element));
				}
				reIndexing(element->childElements());
		  }
	 }
	 updateGeometry();
	 return (m_result && m_argument);
}

void CLink::calc(const stTimeLine &timeLine)
{
	 if(m_argument) m_argument->calc(timeLine);
}

void CLink::onResultDataColorChanged(const QColor &dataColor)
{
	 if(m_argument) m_argument->setDataColor(dataColor);
	 update();
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
	 CElement::updateGeometry();
	 update();
}