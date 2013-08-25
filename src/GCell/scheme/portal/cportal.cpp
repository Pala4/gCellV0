#include "cportal.h"

#include <QPainter>
#include <QAction>
#include <QSignalMapper>

#include "../link/clink.h"
#include "../DataBuffer/cdatabuffer.h"

QPainterPath CPortal::shapeFromPath(const QPainterPath &path, const QPen &pen)
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

void CPortal::updateLinks(void)
{
	foreach(CLink *link, m_links) if(link) link->updateGeometry();
}

QVariant CPortal::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
	switch((int)change)
	{
		case QGraphicsItem::ItemScenePositionHasChanged: updateLinks();	break;
	}

	return CElement::itemChange(change, value);
}

void CPortal::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	Q_UNUSED(event)
	m_hovered = true;
	update();
}

void CPortal::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	Q_UNUSED(event)
}

void CPortal::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	Q_UNUSED(event)
	m_hovered = false;
	update();
}

QRectF CPortal::calcBounds(void)
{
	QRectF boundRect = shape().controlPointRect();
	QRectF portalFormRect = boundRect;

	QFontMetricsF fm(captionFont());
	m_captionRect = boundRect;
	m_captionRect.setWidth(fm.boundingRect(caption()).width() + 2.0);
	m_captionRect.setHeight(fm.boundingRect(caption()).height() + 2.0);

	qreal maxWidth = qMax(boundRect.width(), m_captionRect.width());
	qreal maxHeight = qMax(boundRect.height(), m_captionRect.height());

	switch ((int)portalOrientation())
	{
		case CPortal::Left:
		case CPortal::Right:
			boundRect.setWidth(boundRect.width() + m_captionRect.width() + 2.0);
			boundRect.setHeight(maxHeight);
			m_captionRect.setHeight(maxHeight);
		break;
		case CPortal::Top:
		case CPortal::Bottom:
			boundRect.setHeight(boundRect.height() + m_captionRect.height() + 2.0);
			boundRect.setWidth(maxWidth);
			m_captionRect.setWidth(maxWidth);
		break;
	}

	qreal offsetX = 0.0;
	qreal offsetY = 0.0;
	switch ((int)portalOrientation())
	{
		case CPortal::Left:
			offsetX = m_captionRect.width() + 2.0;
			offsetY = qAbs(boundRect.height() - portalFormRect.height())/2.0;
		break;
		case CPortal::Right:
			offsetY = qAbs(boundRect.height() - portalFormRect.height())/2.0;
			m_captionRect.moveLeft(portalFormRect.width() + 2.0);
		break;
		case CPortal::Top:
			offsetX = qAbs(boundRect.width() - portalFormRect.width())/2.0;
			offsetY = m_captionRect.height() + 2.0;
		break;
		case CPortal::Bottom:
			offsetX = qAbs(boundRect.width() - portalFormRect.width())/2.0;
			m_captionRect.moveTop(portalFormRect.height() + 2.0);
		break;
	}
	m_portalForm.translate(offsetX, offsetY);
	setLinkPos(QPointF(linkPos().x() + offsetX, linkPos().y() + offsetY));

	return boundRect;
}

CPortal::CPortal(QGraphicsItem *parent) : CElement(parent)
{
	setObjectName(QStringLiteral("CPortal"));

	m_portalOrientation = CPortal::Left;
	m_dataColor = QColor(255, 255, 255);
	m_size = 7.0;
	m_hovered = false;
	m_checked = false;
	m_loopBackPortal = false;
	m_dataBuffer = 0;
	m_dataBufferIsReference = false;
	m_orientActionsMapper = 0;
	m_acLeftOrient = 0;
	m_acTopOrient = 0;
	m_acRightOrient = 0;
	m_acBottomOrient = 0;

	m_orientActionsMapper = new QSignalMapper(this);
	m_orientActionsMapper->setObjectName(QStringLiteral("orientActionsMapper"));
	connect(m_orientActionsMapper, SIGNAL(mapped(int)), this, SLOT(setPortalOrientation(int)));

	QActionGroup *acgrOrient = new QActionGroup(this);
	acgrOrient->setObjectName(QStringLiteral("acgrOrient"));

	m_acLeftOrient = new QAction(tr("Left"), this);
	m_acLeftOrient->setObjectName(QStringLiteral("acLeftOrient"));
	m_acLeftOrient->setCheckable(true);
	m_acLeftOrient->setChecked(true);
	connect(m_acLeftOrient, SIGNAL(triggered()), m_orientActionsMapper, SLOT(map()));
	m_orientActionsMapper->setMapping(m_acLeftOrient, (int)CPortal::Left);
	addAction(m_acLeftOrient);
	acgrOrient->addAction(m_acLeftOrient);

	m_acTopOrient = new QAction(tr("Top"), this);
	m_acTopOrient->setObjectName(QStringLiteral("acTopOrient"));
	m_acTopOrient->setCheckable(true);
	connect(m_acTopOrient, SIGNAL(triggered()), m_orientActionsMapper, SLOT(map()));
	m_orientActionsMapper->setMapping(m_acTopOrient, (int)CPortal::Top);
	addAction(m_acTopOrient);
	acgrOrient->addAction(m_acTopOrient);

	m_acRightOrient = new QAction(tr("Right"), this);
	m_acRightOrient->setObjectName(QStringLiteral("acRightOrient"));
	m_acRightOrient->setCheckable(true);
	connect(m_acRightOrient, SIGNAL(triggered()), m_orientActionsMapper, SLOT(map()));
	m_orientActionsMapper->setMapping(m_acRightOrient, (int)CPortal::Right);
	addAction(m_acRightOrient);
	acgrOrient->addAction(m_acRightOrient);

	m_acBottomOrient = new QAction(tr("Bottom"), this);
	m_acBottomOrient->setObjectName(QStringLiteral("acBottomOrient"));
	m_acBottomOrient->setCheckable(true);
	connect(m_acBottomOrient, SIGNAL(triggered()), m_orientActionsMapper, SLOT(map()));
	m_orientActionsMapper->setMapping(m_acBottomOrient, (int)CPortal::Bottom);
	addAction(m_acBottomOrient);
	acgrOrient->addAction(m_acBottomOrient);

	setCaptionFont(QFont("Corier", 7, QFont::Bold));

	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
	setAcceptHoverEvents(true);
}

QPainterPath CPortal::shape(void) const
{
	return shapeFromPath(m_portalForm, QPen());
}

void CPortal::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	CElement::paint(painter, option, widget);

	QBrush brush;
	brush.setStyle(Qt::SolidPattern);
	if(m_hovered)
	{
		brush.setColor(QColor(0, 128, 128, 190));
	}
	else if(isChecked())
	{
		brush.setColor(QColor(128, 0, 255, 190));
	}
	else if(isLoopBackPortal())
	{
		brush.setColor(QColor(0, 0, 0, 255));
	}
	else
	{
		if(dataColor().isValid())
		{
			brush.setColor(dataColor());
		}
		else
		{
			brush.setColor(QColor(255, 255, 255, 0));
		}
	}
	QPen pen;
	pen.setStyle(Qt::SolidLine);
	pen.setColor(QColor(0, 255, 0, 100));

	painter->save();
	painter->setRenderHint(QPainter::Antialiasing);

//	painter->drawRect(boundingRect());
	painter->drawText(m_captionRect, Qt::AlignCenter, caption());

	painter->setBrush(brush);
	painter->setPen(pen);
	painter->drawPath(m_portalForm);
	painter->restore();
}

void CPortal::setPortalOrientation(const CPortal::TPortalOrientation &portalOrientation)
{
	m_portalOrientation = portalOrientation;
	switch((int)m_portalOrientation)
	{
		case CPortal::Left: if(m_acLeftOrient) m_acLeftOrient->setChecked(true); break;
		case CPortal::Top: if(m_acTopOrient) m_acTopOrient->setChecked(true); break;
		case CPortal::Right: if(m_acRightOrient) m_acRightOrient->setChecked(true); break;
		case CPortal::Bottom: if(m_acBottomOrient) m_acBottomOrient->setChecked(true); break;
	}
	updateGeometry();
}

void CPortal::setDataColor(const QColor &dataColor)
{
	if(m_dataColor == dataColor) return;
	m_dataColor = dataColor;
	update();
	emit dataColorChanged(m_dataColor);
}

void CPortal::setChecked(const bool &cheked)
{
	if(m_checked == cheked) return;
	m_checked = cheked;
	update();
}

void CPortal::addLink(CLink *link)
{
	if(!link) return;
	if(m_links.contains(link)) return;

	m_links << link;
}

void CPortal::removeLink(CLink *link)
{
	if(!link || !m_links.contains(link)) return;
	m_links.removeOne(link);
}

CDataBuffer* CPortal::createBuffer(void)
{
	m_dataBufferIsReference = false;
	m_dataBuffer = new CDataBuffer(this);
	m_dataBuffer->setObjectName(QStringLiteral("dataBuffer"));
	return m_dataBuffer;
}

void CPortal::setBuffer(CDataBuffer *dataBuffer)
{
	if(!m_dataBufferIsReference && m_dataBuffer) return;
	m_dataBufferIsReference = true;
	m_dataBuffer = dataBuffer;
}

void CPortal::clearBuffer(void)
{
	if(!m_dataBuffer || m_dataBufferIsReference) return;
	m_dataBuffer->clear();
}

void CPortal::appendBuffer(const stTimeFrame &timeFrame, const qreal &value)
{
	if(m_dataBuffer && !m_dataBufferIsReference) m_dataBuffer->append(timeFrame, value);
}

stData CPortal::bufferData(const int &index)
{
	if(!m_dataBuffer) return stData();
	if(isLoopBackPortal())
	{
		return m_dataBuffer->last();
	}
	return m_dataBuffer->data(index);
}

bool CPortal::isBufferDataExist(const stTimeFrame &frame)
{
	if(!m_dataBuffer) return false;
	return m_dataBuffer->containsFrame(frame);
}

void CPortal::beforeCalc(const qreal &startTime, const qreal &timeStep, const qreal &endTime)
{
	Q_UNUSED(startTime)
	Q_UNUSED(timeStep)
	Q_UNUSED(endTime)

	clearBuffer();
    if(isLoopBackPortal()) setLoopBackPortal(false);
}

void CPortal::setPortalOrientation(const int &portalOrientation)
{
	setPortalOrientation((CPortal::TPortalOrientation)portalOrientation);
}

void CPortal::updateGeometry(void)
{
	prepareGeometryChange();
	m_portalForm = calcPortalForm();
	CElement::updateGeometry();
	emit geometryChanged();
	update();
}