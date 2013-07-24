#include "calgorithm.h"

#include <QFont>
#include <QFontMetrics>
#include <QPainter>

#include "cscheme.h"
#include "cportal.h"
#include "cargument.h"
#include "cresult.h"
#include "elementlistutil.h"

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

	int numLP = 0, numTP = 0, numRP = 0, numBP = 0;
	foreach(CPortal *portal, m_portals)
	{
		if(!portal) continue;
		switch((int)portal->portalOrientation())
		{
			case CPortal::Left: ++numLP; break;
			case CPortal::Top: ++numTP; break;
			case CPortal::Right: ++numRP; break;
			case CPortal::Bottom: ++numBP; break;
		}
	}

	m_boundingRect.setWidth(qMax(qMax(numTP, numBP)*7.0, m_boundingRect.width()));
	m_boundingRect.setHeight(qMax(qMax(numLP, numRP)*7.0, m_boundingRect.height()));

	update();
}

void CAlgorithm::placePortals(void)
{
	int numLP = 0, numTP = 0, numRP = 0, numBP = 0;
	foreach(CPortal *portal, m_portals)
	{
		if(!portal) continue;
		switch((int)portal->portalOrientation())
		{
			case CPortal::Left: ++numLP; break;
			case CPortal::Top: ++numTP; break;
			case CPortal::Right: ++numRP; break;
			case CPortal::Bottom: ++numBP; break;
		}
	}

	QPointF dLP(-5.0, (m_boundingRect.height() - numLP*7.0)/2.0);
	QPointF dTP((m_boundingRect.width() - numTP*7.0)/2.0, -5.0);
	QPointF dRP(m_boundingRect.width(), (m_boundingRect.height() - numRP*7.0)/2.0);
	QPointF dBP((m_boundingRect.width() - numBP*7.0)/2.0, m_boundingRect.height());

	foreach(CPortal *portal, m_portals)
	{
		if(!portal) continue;
		switch((int)portal->portalOrientation())
		{
			case CPortal::Left: portal->setPos(dLP); dLP.setY(dLP.y() + 7.0); break;
			case CPortal::Top: portal->setPos(dTP); dTP.setX(dTP.x() + 7.0); break;
			case CPortal::Right: portal->setPos(dRP); dRP.setY(dRP.y() + 7.0); break;
			case CPortal::Bottom: portal->setPos(dBP); dBP.setX(dBP.x() + 7.0);	break;
		}
	}
}

void CAlgorithm::addPortal(CPortal *portal)
{
	if(!portal) return;
	if(m_portals.contains(portal)) return;

	m_portals << portal;
	connect(portal, SIGNAL(destroyed(QObject*)), this, SLOT(onPortalDestroyed(QObject*)));
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
	addPortal(res);

	return res;
}

CAlgorithm::CAlgorithm(QGraphicsItem *parent) : CElement(parent)
{
	setObjectName(QStringLiteral("CAlgorithm"));

	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemIsMovable, false);
}

void CAlgorithm::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)

	painter->save();
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setRenderHint(QPainter::TextAntialiasing, true);

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
	painter->setPen(pen);

	painter->drawRect(boundingRect());
	painter->drawText(boundingRect(), Qt::AlignCenter, name());

	painter->restore();
}

void CAlgorithm::calc(const int &timeFrame)
{
	QList<CArgument*> args = getElements<CArgument*, CPortal*>(portals());
	foreach(CArgument *arg, args)
	{
		if(!arg) continue;
		if(!arg->isLoopBackPortal() && !arg->isDataReady(timeFrame)) return;
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