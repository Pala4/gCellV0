#include "calgorithm.h"

#include <QFont>
#include <QFontMetrics>
#include <QPainter>

#include "../cscheme.h"
#include "../portal/cportal.h"
#include "../portal/cargument.h"
#include "../portal/cresult.h"
#include "../elementlistutil.h"

void CAlgorithm::calcBounds(void)
{
	prepareGeometryChange();
	QFont f("Corier", 8, QFont::Bold);
	QFontMetricsF fm(f);
	m_innerBound.setWidth(fm.boundingRect(name()).width());
	m_innerBound.setHeight(fm.boundingRect(name()).height());
	m_boundingRect = m_innerBound;
	if(m_portals.isEmpty())
	{
		update();
		return;
	}

	qreal LPHeight = 0.0, TPWidth = 0.0, RPHeight = 0.0, BPWidth = 0.0;
	foreach(CPortal *portal, m_portals)
	{
		if(!portal) continue;
		switch((int)portal->portalOrientation())
		{
			case CPortal::Left: LPHeight += portal->boundingRect().height() + m_portalSpace; break;
			case CPortal::Top: TPWidth += portal->boundingRect().width() + m_portalSpace; break;
			case CPortal::Right: RPHeight += portal->boundingRect().height() + m_portalSpace; break;
			case CPortal::Bottom: BPWidth += portal->boundingRect().width() + m_portalSpace; break;
		}
	}

	m_boundingRect.setWidth(qMax(qMax(TPWidth, BPWidth)/* - m_portalSpace*/, m_boundingRect.width()));
	m_boundingRect.setHeight(qMax(qMax(LPHeight, RPHeight)/* - m_portalSpace*/, m_boundingRect.height()));

	update();
}

void CAlgorithm::placePortals(void)
{
	qreal LPHeight = 0.0, TPWidth = 0.0, RPHeight = 0.0, BPWidth = 0.0;
	foreach(CPortal *portal, m_portals)
	{
		if(!portal) continue;
		switch((int)portal->portalOrientation())
		{
			case CPortal::Left: LPHeight += portal->boundingRect().height() + m_portalSpace; break;
			case CPortal::Top: TPWidth += portal->boundingRect().width() + m_portalSpace; break;
			case CPortal::Right: RPHeight += portal->boundingRect().height() + m_portalSpace; break;
			case CPortal::Bottom: BPWidth += portal->boundingRect().width() + m_portalSpace; break;
		}
	}

	QPointF dLP(0.0, (m_boundingRect.height() - LPHeight + m_portalSpace)/2.0);
	QPointF dTP((m_boundingRect.width() - TPWidth + m_portalSpace)/2.0, 0.0);
	QPointF dRP(m_boundingRect.width() + m_portalMargin, (m_boundingRect.height() - RPHeight + m_portalSpace)/2.0);
	QPointF dBP((m_boundingRect.width() - BPWidth + m_portalSpace)/2.0, m_boundingRect.height() + m_portalMargin);

	foreach(CPortal *portal, m_portals)
	{
		if(!portal) continue;
		switch((int)portal->portalOrientation())
		{
			case CPortal::Left:
			{
				portal->setPos(QPointF(-portal->boundingRect().width() - m_portalMargin, dLP.y()));
				dLP.setY(dLP.y() + portal->boundingRect().height() + m_portalSpace);
				break;
			}
			case CPortal::Top:
			{
				portal->setPos(QPointF(dTP.x(), -portal->boundingRect().height() - m_portalMargin));
				dTP.setX(dTP.x() + portal->boundingRect().width() + m_portalSpace);
				break;
			}
			case CPortal::Right:
				portal->setPos(dRP);
				dRP.setY(dRP.y() + portal->boundingRect().height() + m_portalSpace);
			break;
			case CPortal::Bottom:
				portal->setPos(dBP);
				dBP.setX(dBP.x() + portal->boundingRect().width() + m_portalSpace);
			break;
		}
	}
}

void CAlgorithm::addPortal(CPortal *portal)
{
	if(!portal) return;
	if(m_portals.contains(portal)) return;

	m_portals << portal;
	connect(portal, SIGNAL(destroyed(QObject*)), this, SLOT(onPortalDestroyed(QObject*)));
	connect(portal, SIGNAL(geometryChanged()), this, SLOT(updateGeometry()));
	updateGeometry();
}

CArgument* CAlgorithm::addArgument(const QString &name)
{
	CArgument *arg = new CArgument(this);
	arg->setPortalOrientation(CPortal::Left);
	arg->setName(name);
	arg->setNomber(generateNomber<CPortal*, CArgument*>(m_portals));
	addPortal(arg);

	return arg;
}

CResult* CAlgorithm::addResult(const QString &name)
{
	CResult *res = new CResult(this);
	res->setPortalOrientation(CPortal::Right);
	res->setName(name);
	res->setNomber(generateNomber<CPortal*, CResult*>(m_portals));
	res->createBuffer();
	addPortal(res);

	return res;
}

CAlgorithm::CAlgorithm(QGraphicsItem *parent) : CElement(parent)
{
	setObjectName(QStringLiteral("CAlgorithm"));

	m_portalSpace = 5.0;
	m_portalMargin = 2.0;

	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemIsMovable, false);
}

void CAlgorithm::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)

	QPen pen;
	pen.setColor(Qt::black);
	if(isSelected())
	{
		pen.setWidthF(2.0);
	}
	else
	{
		pen.setWidthF(0.0);
	}

	painter->save();
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setRenderHint(QPainter::TextAntialiasing, true);
	painter->setPen(pen);
	painter->drawRect(boundingRect());
	painter->drawText(boundingRect(), Qt::AlignCenter, name());
	painter->restore();
}

QList<CArgument*> CAlgorithm::arguments(void)
{
	return getElements<CArgument*, CPortal*>(portals());
}

QList<CResult*> CAlgorithm::results(void)
{
	return getElements<CResult*, CPortal*>(portals());
}

void CAlgorithm::calc(const int &timeFrame)
{
	QList<CArgument*> args = getElements<CArgument*, CPortal*>(portals());
	foreach(CArgument *arg, args)
	{
		if(!arg) continue;
		if(!arg->isLoopBackPortal() && !arg->isBufferDataReady(timeFrame)) return;
	}
	proced(timeFrame);
	QList<CResult*> ress = getElements<CResult*, CPortal*>(portals());
	foreach(CResult *res, ress)
	{
		if(res) res->calc(timeFrame);
	}
}

void CAlgorithm::onPortalDestroyed(QObject *objPortal)
{
	if(!m_portals.contains((CPortal*)objPortal)) return;
	m_portals.removeOne((CPortal*)objPortal);
	updateGeometry();
}

void CAlgorithm::updateGeometry(void)
{
	calcBounds();
	placePortals();
}