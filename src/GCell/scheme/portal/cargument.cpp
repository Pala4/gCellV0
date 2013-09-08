#include "cargument.h"

#include "../algorithm/calgorithm.h"

QPainterPath CArgument::calcPortalForm(void)
{
	QRectF arrowRect(0.0, 0.0, size(), size());
	QPointF p0, p1, p2, pLink;

	switch(portalOrientation())
	{
		case CPortal::Left:
			p0 = arrowRect.topLeft();
			p1 = QPointF(arrowRect.right(), arrowRect.center().y());
			p2 = arrowRect.bottomLeft();
			pLink = QPointF(p0.x(), p1.y());
		break;
		case CPortal::Top:
			p0 = arrowRect.topLeft();
			p1 = QPointF(arrowRect.center().x(), arrowRect.bottom());
			p2 = arrowRect.topRight();
			pLink = QPointF(p1.x(), p0.y());
		break;
		case CPortal::Right:
			p0 = arrowRect.topRight();
			p1 = QPointF(arrowRect.left(), arrowRect.center().y());
			p2 = arrowRect.bottomRight();
			pLink = QPointF(p0.x(), p1.y());
		break;
		case CPortal::Bottom:
			p0 = arrowRect.bottomLeft();
			p1 = QPointF(arrowRect.center().x(), arrowRect.top());
			p2 = arrowRect.bottomRight();
			pLink = QPointF(p1.x(), p0.y());
		break;
		case CPortal::NoOrientation: break;
	}
	setLinkPos(pLink);

	QPainterPath m_arrowForm = QPainterPath();
	m_arrowForm.moveTo(p0);
	m_arrowForm.lineTo(p1);
	m_arrowForm.lineTo(p2);
	m_arrowForm.closeSubpath();
	return m_arrowForm;
}

CArgument::CArgument(QGraphicsItem *parent) : CPortal(parent)
{
	setObjectName(QStringLiteral("CArgument"));

	setPortalOrientation(CPortal::Left);
	setDefaultName(tr("arg"));
}

void CArgument::calc(const stTimeLine &timeLine)
{
	if(isLoopBackPortal()) return;
	CElement *nextElement = hostElement();
	if(nextElement) nextElement->calc(timeLine);
}