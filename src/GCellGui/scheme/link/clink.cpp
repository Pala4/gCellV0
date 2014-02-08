#include "clink.h"

#include <QPainter>

#include "clinkenv.h"
#include "clinksegment.h"
#include "../portal/cresult.h"
#include "../portal/cargument.h"
#include "../cscheme.h"

QPainterPath CLink::shapeFromPath(const QPainterPath &path, const QPen &pen)
{
    if (path == QPainterPath())
        return path;

	QPainterPathStroker ps;
	ps.setCapStyle(pen.capStyle());

	qreal penWidth = pen.widthF() <= 8.0 ? 10.0 : pen.widthF() + 2.0;
	ps.setWidth(penWidth);

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
    if (linkEnv() != nullptr) {
		pointsList << linkEnv()->firstPoint();
		pointsList << linkEnv()->firstFixedPoint();
	}
    if (m_linkSegments != nullptr)
        pointsList << m_linkSegments->points();
    if (linkEnv() != nullptr) {
		pointsList << linkEnv()->secondFixedPoint();
		pointsList << linkEnv()->secondPoint();
	}
    if (pointsList.count() >= 2) {
        for (int ci = 0; ci < pointsList.count() - 1; ++ci) {
			m_path.moveTo(pointsList.at(ci));
			m_path.lineTo(pointsList.at(ci + 1));
		}
	}

	emit formChanged();
}

QRectF CLink::calcBounds()
{
	return shape().controlPointRect().adjusted(-1.0, -1.0, 1.0, 1.0);
}

QPointF CLink::captionEditorPosition()
{
    return (m_linkSegments != nullptr) ? m_linkSegments->segment(CLinkSegment::Medium)->center()
                                       : CElement::captionEditorPosition();
}

CLink::CLink(QGraphicsItem *parent) : CElement(parent)
{
	 setObjectName(QStringLiteral("CLink"));

     m_result = nullptr;
     m_argument = nullptr;
	 m_distFromPortal = 10.0;
     m_linkEnv = nullptr;
     m_linkSegments = nullptr;

	 m_linkEnv = new CLinkEnv(this);
	 m_linkSegments = new CLinkSegments(this);

     if (captionEditor() != nullptr) {
		 captionEditor()->setFlag(QGraphicsItem::ItemIsMovable);
	 }

	 setDefaultName(tr("link"));
	 setCaptionVisible(false);
	 setZValue(0.0);
}

CLink::~CLink()
{
     if (m_result != nullptr)
         m_result->removeLink(this);
     if (m_argument != nullptr) {
          m_argument->setBuffer(nullptr);
		  m_argument->setDataColor(QColor(255, 255, 255, 0));
		  m_argument->removeLink(this);
	 }
}

void CLink::setDistFromPortal(const qreal &distFromPortal)
{
    if (m_distFromPortal == distFromPortal)
        return;

	m_distFromPortal = distFromPortal;
	updateGeometry();
}

QPointF CLink::firstPos()
{
    return (m_result != nullptr) ? m_result->mapToScene(m_result->linkPos()) : QPointF();
}

CPortal::TPortalOrientation CLink::firstOrientation()
{
    return (m_result != nullptr) ? m_result->portalOrientation() : CPortal::NoOrientation;
}

QRectF CLink::firstFigure()
{
	QRectF ff;
    if ((m_result != nullptr) && (m_result->hostElement() != nullptr)) {
		ff = QRectF(m_result->hostElement()->pos(), m_result->hostElement()->boundingRect().size());
	}

	return ff;
}

QPointF CLink::secondPos()
{
    return (m_argument != nullptr) ? m_argument->mapToScene(m_argument->linkPos()) : QPointF();
}

CPortal::TPortalOrientation CLink::secondOrientation()
{
    return (m_argument != nullptr) ? m_argument->portalOrientation() : CPortal::NoOrientation;
}

QRectF CLink::secondFigure()
{
	QRectF sf;
    if ((m_argument != nullptr) && (m_argument->hostElement() != nullptr)) {
        sf = QRectF(m_argument->hostElement()->pos(),
                    m_argument->hostElement()->boundingRect().size());
	}

	return sf;
}

QPainterPath CLink::shape() const
{
	 return shapeFromPath(m_path, QPen());
}

void CLink::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	 CElement::paint(painter, option, widget);

	 QPen pen;
     if (isSelected()) {
		  pen.setWidthF(2.0);
	 }
     if (m_result != nullptr)
         pen.setColor(m_result->dataColor());

	 painter->save();
	 painter->setPen(pen);
	 painter->setRenderHint(QPainter::Antialiasing);
	 painter->drawPath(m_path);
	 painter->restore();
}

QString CLink::resultID()
{
     if (m_result != nullptr)
         return m_result->id();

	 return m_resultID;
}

void CLink::setResultID(const QString &resultID)
{
     if (scheme() != nullptr) {
		  setResult(qobject_cast<CResult*>(scheme()->element(resultID)));
     } else {
		  m_resultID = resultID;
	 }
}

QString CLink::argumentID()
{
     if (m_argument != nullptr)
         return m_argument->id();

	 return m_argumentID;
}

void CLink::setArgumentID(const QString &argumentID)
{
     if (scheme() != nullptr) {
		  setArgument(qobject_cast<CArgument*>(scheme()->element(argumentID)));
     } else {
		  m_argumentID = argumentID;
	 }
}

const QPointF CLink::firstOffset() const
{
    if (m_linkSegments == nullptr)
        return QPointF();

	CLinkSegment *ls = m_linkSegments->segment(CLinkSegment::First);
    if (ls == nullptr)
        return QPointF();

	return ls->managedOffset();
}

void CLink::setFirstOffset(const QPointF &firstOffset)
{
    if (m_linkSegments == nullptr)
        return;

	CLinkSegment *ls = m_linkSegments->segment(CLinkSegment::First);
    if (ls != nullptr)
        ls->setManagedOffset(firstOffset);
}

const QPointF CLink::medOffset() const
{
    if (m_linkSegments == nullptr)
        return QPointF();

	CLinkSegment *ls = m_linkSegments->segment(CLinkSegment::Medium);
    if (ls == nullptr)
        return QPointF();

	return ls->managedOffset();
}

void CLink::setMedOffset(const QPointF &medOffset)
{
    if (m_linkSegments == nullptr)
        return;

	CLinkSegment *ls = m_linkSegments->segment(CLinkSegment::Medium);
    if (ls != nullptr)
        ls->setManagedOffset(medOffset);
}

const QPointF CLink::secondOffset() const
{
    if (m_linkSegments == nullptr)
        return QPointF();

	CLinkSegment *ls = m_linkSegments->segment(CLinkSegment::Second);
    if (ls == nullptr)
        return QPointF();

	return ls->managedOffset();
}

void CLink::setSecondOffset(const QPointF &secondOffset)
{
    if (m_linkSegments == nullptr)
        return;

	CLinkSegment *ls = m_linkSegments->segment(CLinkSegment::Second);
    if (ls != nullptr)
        ls->setManagedOffset(secondOffset);
}

void CLink::setResult(CResult *result)
{
     if ((m_result != nullptr) && (m_result == result))
         return;
     if (m_result != nullptr) {
          disconnect(m_result, SIGNAL(dataColorChanged(QColor)),
                     this, SLOT(onResultDataColorChanged(QColor)));
          disconnect(m_result, SIGNAL(destroyed()), this, SLOT(onResultDestroyed()));
		  m_result->removeLink(this);
          if (m_argument != nullptr) {
                m_argument->setBuffer(nullptr);
				m_argument->setDataColor(QColor());
		  }
	 }
	 m_result = result;
     if (m_result != nullptr) {
          connect(m_result, SIGNAL(dataColorChanged(QColor)),
                  this, SLOT(onResultDataColorChanged(QColor)));
          connect(m_result, SIGNAL(destroyed()), this, SLOT(onResultDestroyed()));
		  m_result->addLink(this);
          if (m_argument != nullptr) {
				m_argument->setBuffer(result->buffer());
				m_argument->setDataColor(result->dataColor());
		  }
	 }
	 updateGeometry();
}

void CLink::setArgument(CArgument *argument)
{
     if ((m_argument != nullptr) && (m_argument == argument))
         return;
     if (m_argument != nullptr) {
          disconnect(m_argument, SIGNAL(destroyed()), this, SLOT(onArgumentDestroyed()));
		  m_argument->removeLink(this);
          if (m_argument != nullptr) {
                m_argument->setBuffer(nullptr);
				m_argument->setDataColor(QColor());
		  }
	 }
	 m_argument = argument;
     if (m_argument != nullptr) {
          connect(m_argument, SIGNAL(destroyed()), this, SLOT(onArgumentDestroyed()));
		  m_argument->addLink(this);
          if (m_result != nullptr) {
				m_argument->setBuffer(m_result->buffer());
				m_argument->setDataColor(m_result->dataColor());
		  }
	 }
	 updateGeometry();
}

QList<CPortal*> CLink::portals()
{
	QList<CPortal*> prts;

    if (m_argument != nullptr)
        prts << m_argument;
    if (m_result != nullptr)
        prts << m_result;

	return prts;
}

bool CLink::reIndexing(const QList<CElement*> &elements)
{
     if ((m_result == nullptr) || (m_argument == nullptr)) {
          foreach(CElement *element, elements) {
              //checking of the exit condition
              if ((m_result != nullptr) && (m_argument != nullptr))
                  return true;
              if (element == nullptr)
                  continue;
              if ((m_result == nullptr) && (element->id() == m_resultID)) {
                  setResult(qobject_cast<CResult*>(element));
              }
              if ((m_argument == nullptr) && (element->id() == m_argumentID)) {
                  setArgument(qobject_cast<CArgument*>(element));
              }

              reIndexing(element->childElements());
          }
	 }

	 updateGeometry();
	 return (m_result && m_argument);
}

void CLink::calc(const unsigned long long &ullTFIndex, const long double &ldblTimeFrame,
                 const long double &ldblStartTime, const long double &ldblTimeStep,
                 const long double &ldblEndTime)
{
     if (m_argument != nullptr)
         m_argument->calc(ullTFIndex, ldblTimeFrame, ldblStartTime, ldblTimeStep, ldblEndTime);
}

void CLink::onResultDataColorChanged(const QColor &dataColor)
{
     if (m_argument != nullptr)
         m_argument->setDataColor(dataColor);

	 update();
}

void CLink::onResultDestroyed()
{
     m_result = nullptr;
	 deleteLater();
}

void CLink::onArgumentDestroyed()
{
     m_argument = nullptr;
	 deleteLater();
}

void CLink::updateGeometry()
{
	 prepareGeometryChange();
	 calcLink();
	 CElement::updateGeometry();
	 update();
}