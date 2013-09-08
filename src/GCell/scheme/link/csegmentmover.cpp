#include "csegmentmover.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>

#include "clink.h"
#include "clinksegment.h"

/*!
 * \class CLinkMoverPart
 */
void CLinkMoverPart::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if(event->buttons() & Qt::LeftButton)
	{
		m_moved = true;
		m_dP = event->scenePos() - pos();
	}
	update();

	QGraphicsItem::mousePressEvent(event);
}

void CLinkMoverPart::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if(m_moved && m_segment)
	{
		if(m_segment->isHorizontal())
		{
			qreal newYOffset = (event->scenePos().y() - m_dP.y()) - m_segment->managedCoord().managedCoord();
			m_segment->setManagedYOffset(newYOffset);
		}
		else if(m_segment->isVertical())
		{
			qreal newXOffset = (event->scenePos().x() - m_dP.x()) - m_segment->managedCoord().managedCoord();
			m_segment->setManagedXOffset(newXOffset);
		}

		emit stateChanged();
	}

	QGraphicsObject::mouseMoveEvent(event);
}

void CLinkMoverPart::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	m_moved = false;
	m_dP = QPointF();
	update();

	QGraphicsItem::mouseReleaseEvent(event);
}

CLinkMoverPart::CLinkMoverPart(QGraphicsItem *parent, CSegmentMover *segmentMover) : QGraphicsObject(parent)
{
	setObjectName(QLatin1String("CLinkMoverPart"));

	m_segmentMover = 0;
	m_segment = 0;

	m_moved = false;

	setSegmentMover(segmentMover);

	setFlag(QGraphicsItem::ItemIsMovable);
	setAcceptHoverEvents(true);
	setAcceptedMouseButtons(Qt::LeftButton);
	setVisible(true);
	setCursor(QCursor(Qt::SizeAllCursor));
}

void CLinkMoverPart::setSegmentMover(CSegmentMover *segmentMover)
{
	if(m_segmentMover && (m_segmentMover == segmentMover)) return;

	m_segmentMover = segmentMover;

	setup();
}

void CLinkMoverPart::setSegment(CLinkSegment *segment)
{
	if(m_segment && (m_segment == segment)) return;

	m_segment = segment;

	setup();
}

QRectF CLinkMoverPart::boundingRect(void) const
{
	if(!m_segmentMover) return QRectF(0.0, 0.0, 5.0, 5.0);

	qreal size = m_segmentMover->partSize();
	return QRectF(0.0, 0.0, size, size);
}

void CLinkMoverPart::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(widget)

	QColor penColor;
	QColor brushColor;

	if(!m_segmentMover)
	{
		penColor = QColor(0, 0, 255, 128);
		brushColor = QColor(55, 55, 255, 128);
	}
	else
	{
		if(option->state & QStyle::State_MouseOver && !(option->state & QStyle::State_Sunken))
		{
			penColor = m_segmentMover->borderHoverColor();
			brushColor = m_segmentMover->fillHoverColor();
		}
		else if(option->state & QStyle::State_Sunken)
		{
			penColor = m_segmentMover->borderMovedColor();
			brushColor = m_segmentMover->fillMovedColor();
		}
		else
		{
			penColor = m_segmentMover->borderNormalColor();
			brushColor = m_segmentMover->fillNormalColor();
		}
	}

	QPen pen;
	QBrush brush;

	pen.setStyle(Qt::SolidLine);
	pen.setColor(penColor);
	brush.setStyle(Qt::SolidPattern);
	brush.setColor(brushColor);

	painter->save();

	painter->setPen(pen);
	painter->setBrush(brush);

	painter->drawRect(boundingRect());

	painter->restore();
}

void CLinkMoverPart::setup(void)
{
	prepareGeometryChange();

	if(m_segment)
	{
		QRectF br = boundingRect();
		br.moveCenter(m_segment->center());

		setPos(br.topLeft());

		if(m_segment->isHorizontal())
		{
			setCursor(QCursor(Qt::SizeVerCursor));
		}
		else if(m_segment->isVertical())
		{
			setCursor(QCursor(Qt::SizeHorCursor));
		}
		else
		{
			setCursor(QCursor(Qt::SizeAllCursor));
		}
	}

	update();
}

/*!
 * \class CLinkMover
 */
void CSegmentMover::setupParts(void)
{
	foreach(CLinkMoverPart *part, m_parts)
	{
		if(part) part->setup();
	}
}

CSegmentMover::CSegmentMover(QObject *parent) : QObject(parent)
{
	setObjectName(QStringLiteral("CSegmentMover"));

	m_partSize = 5.0;
	m_borderNormalColor = QColor(0, 0, 255, 128);
	m_fillNormalColor = QColor(55, 55, 255, 128);
	m_borderHoverColor = QColor(0, 255, 0, 128);
	m_fillHoverColor = QColor(55, 255, 55, 128);
	m_borderMovedColor = QColor(255, 0, 0, 128);
	m_fillMovedColor = QColor(255, 55, 55, 128);

	m_link = 0;
}

void CSegmentMover::setLink(CLink *link)
{
	if(m_link && (m_link == link)) return;

	if(m_link)
	{
		disconnect(m_link, SIGNAL(formChanged()), this, SLOT(onLinkFormChanged()));

		foreach(CLinkMoverPart *part, m_parts)
		{
			if(part) delete part;
		}
		m_parts.clear();
	}

	m_link = link;

	if(m_link && m_link->scene())
	{
		connect(m_link, SIGNAL(formChanged()), this, SLOT(onLinkFormChanged()));

		CLinkSegments *linkSegments = m_link->linkSegments();
		if(linkSegments)
		{
			foreach(CLinkSegment *segment, linkSegments->segmentList())
			{
				if(segment)
				{
					CLinkMoverPart *part = new CLinkMoverPart(m_link, this);
					part->setSegment(segment);
					connect(part, SIGNAL(stateChanged()), this, SLOT(onPartStateChanged()));
					m_parts << part;
//					m_link->scene()->addItem(part);
//					m_link->stackBefore(part);
				}
			}
		}
	}
}

void CSegmentMover::onPartStateChanged(void)
{
	if(m_link) m_link->updateGeometry();
}

void CSegmentMover::onLinkFormChanged(void)
{
	setupParts();
}