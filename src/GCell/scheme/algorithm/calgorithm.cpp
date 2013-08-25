#include "calgorithm.h"

#include <QFontMetrics>
#include <QPainter>

#include "../cscheme.h"
#include "../portal/cportal.h"
#include "../portal/cargument.h"
#include "../portal/cresult.h"
#include "../elementlistutil.h"

QRectF CAlgorithm::calcBounds(void)
{
	prepareGeometryChange();
	QFontMetricsF fm(captionFont());
	QRectF innerBound;
	innerBound.setWidth(fm.boundingRect(caption()).adjusted(-2.0, -2.0, 2.0, 2.0).width());
	innerBound.setHeight(fm.boundingRect(caption()).adjusted(-2.0, -2.0, 2.0, 2.0).height());

	QRectF boundRect;
	boundRect = innerBound;
	if(m_portals.isEmpty())
	{
		update();
		return boundRect;
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

	boundRect.setWidth(qMax(qMax(TPWidth, BPWidth), boundRect.width()));
	boundRect.setHeight(qMax(qMax(LPHeight, RPHeight), boundRect.height()));

	return boundRect;
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

	QRectF boundRect = boundingRect();

	QPointF dLP(0.0, (boundRect.height() - LPHeight + m_portalSpace)/2.0);
	QPointF dTP((boundRect.width() - TPWidth + m_portalSpace)/2.0, 0.0);
	QPointF dRP(boundRect.width() + m_portalMargin, (boundRect.height() - RPHeight + m_portalSpace)/2.0);
	QPointF dBP((boundRect.width() - BPWidth + m_portalSpace)/2.0, boundRect.height() + m_portalMargin);

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
    if(m_portals.contains(portal->id())) return;
    if(m_portals.values().contains(portal)) return;

    m_portals[portal->id()] = portal;
	connect(portal, SIGNAL(destroyed(QObject*)), this, SLOT(onPortalDestroyed(QObject*)));
	connect(portal, SIGNAL(geometryChanged()), this, SLOT(updateGeometry()));
    updateGeometry();

	emit portalAdded(portal);
}

void CAlgorithm::removePortal(const QString &id)
{
    if(id.isEmpty()) return;
    if(!m_portals.contains(id)) return;

    CPortal *portal = m_portals[id];
	if(portal)
	{
		portal->deleteLater();
	}
}

CArgument* CAlgorithm::addArgument(const QString &name)
{
	CArgument *arg = new CArgument(this);
	arg->setPortalOrientation(CPortal::Left);
	arg->setName(name);
    arg->setNomber(generateNomber<CPortal*, CArgument*>(m_portals.values()));
	addPortal(arg);

	return arg;
}

CResult* CAlgorithm::addResult(const QString &name)
{
	CResult *res = new CResult(this);
	res->setPortalOrientation(CPortal::Right);
	res->setDataColor(QColor(qRound(qrand()*255.0/RAND_MAX), qRound(qrand()*255.0/RAND_MAX), qRound(qrand()*255.0/RAND_MAX), 180));
	res->setName(name);
    res->setNomber(generateNomber<CPortal*, CResult*>(m_portals.values()));
	res->createBuffer();
	addPortal(res);

	return res;
}

void CAlgorithm::clearResults(void)
{
    QList<CResult*> ress = getElements<CResult*, CPortal*>(m_portals.values());
	foreach(CResult *res, ress)
	{
		if(res) res->deleteLater();
	}
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
	CElement::paint(painter, option, widget);

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
	painter->drawText(boundingRect(), Qt::AlignCenter, caption());
    painter->restore();
}

CResult* CAlgorithm::result(const QString &id)
{
    if(id.isEmpty()) return 0;
    if(!m_portals.contains(id)) return 0;

	return qobject_cast<CResult*>(m_portals[id]);
}

QList<CPortal*> CAlgorithm::argPortals(void)
{
	return getElements<CPortal*, CPortal*>(portals(), CArgument::staticMetaObject.className());
}

QList<CPortal*> CAlgorithm::resPortals(void)
{
	return getElements<CPortal*, CPortal*>(portals(), CResult::staticMetaObject.className());
}

QList<CArgument*> CAlgorithm::arguments(void)
{
	return getElements<CArgument*, CPortal*>(portals());
}

QList<CResult*> CAlgorithm::results(void)
{
	return getElements<CResult*, CPortal*>(portals());
}

CElement* CAlgorithm::createElement(const QString &typeID)
{
	if(typeID == CResult::staticMetaObject.className())
	{
		return addResult(QString());
	}
	else if(typeID == CArgument::staticMetaObject.className())
	{
		return addArgument(QString());
	}

	return 0;
}

void CAlgorithm::calc(const stTimeLine &timeLine)
{
	QList<CArgument*> args = getElements<CArgument*, CPortal*>(portals());
	foreach(CArgument *arg, args)
	{
		if(!arg) continue;
		if(!arg->isUsed()) continue;
		if(!arg->isLoopBackPortal() && !arg->isBufferDataExist(timeLine.timeFrame)) return;
	}
	proced(timeLine);
	QList<CResult*> ress = getElements<CResult*, CPortal*>(portals());
	foreach(CResult *res, ress)
	{
		if(res) res->calc(timeLine);
	}
}

void CAlgorithm::onPortalDestroyed(QObject *objPortal)
{
    if(!m_portals.values().contains((CPortal*)objPortal)) return;
    m_portals.remove(m_portals.key((CPortal*)objPortal));
	updateGeometry();
}

void CAlgorithm::updateGeometry(void)
{
	CElement::updateGeometry();
	placePortals();
}