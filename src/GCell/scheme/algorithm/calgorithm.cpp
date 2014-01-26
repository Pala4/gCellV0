#include "calgorithm.h"

#include <QFontMetrics>
#include <QPainter>

#include "../cscheme.h"
#include "../portal/cportal.h"
#include "../portal/cargument.h"
#include "../portal/cresult.h"
#include "../elementlistutil.h"

void CAlgorithm::registerPortal(CPortal *portal)
{
    if (portal == nullptr)
        return;

	connect(portal, SIGNAL(destroyed(QObject*)), this, SLOT(onPortalDestroyed(QObject*)));
	connect(portal, SIGNAL(geometryChanged()), this, SLOT(updateGeometry()));
	updateGeometry();

	emit portalAdded(portal);
}

QRectF CAlgorithm::calcBounds()
{
	prepareGeometryChange();
	QRectF elementBounds = CElement::calcBounds();

//	QFontMetricsF fm(captionFont());
	QRectF innerBound;
	innerBound.setWidth(elementBounds./*adjusted(-2.0, -2.0, 2.0, 2.0).*/width());
	innerBound.setHeight(elementBounds./*adjusted(-2.0, -2.0, 2.0, 2.0).*/height());

	QRectF boundRect;
	boundRect = innerBound;
    if (portals().isEmpty()) {
		update();
		return boundRect;
	}

    qreal LPHeight = 0.0;
    qreal TPWidth = 0.0;
    qreal RPHeight = 0.0;
    qreal BPWidth = 0.0;
    foreach (CPortal *portal, portals()) {
        if (portal == nullptr)
            continue;
        switch (portal->portalOrientation()) {
        case CPortal::Left:
            LPHeight += portal->boundingRect().height() + m_portalSpace;
            break;
        case CPortal::Top:
            TPWidth += portal->boundingRect().width() + m_portalSpace;
            break;
        case CPortal::Right:
            RPHeight += portal->boundingRect().height() + m_portalSpace;
            break;
        case CPortal::Bottom:
            BPWidth += portal->boundingRect().width() + m_portalSpace;
            break;
        default:
            break;
        }
	}

	boundRect.setWidth(qMax(qMax(TPWidth, BPWidth), boundRect.width()));
	boundRect.setHeight(qMax(qMax(LPHeight, RPHeight), boundRect.height()));

	return boundRect.adjusted(-1.0, -1.0, 1.0, 1.0);
}

void CAlgorithm::placePortals()
{
    qreal LPHeight = 0.0;
    qreal TPWidth = 0.0;
    qreal RPHeight = 0.0;
    qreal BPWidth = 0.0;

    foreach (CPortal *portal, portals()) {
        if (portal == nullptr)
            continue;
        switch (portal->portalOrientation()) {
        case CPortal::Left:
            LPHeight += portal->boundingRect().height() + m_portalSpace;
            break;
        case CPortal::Top:
            TPWidth += portal->boundingRect().width() + m_portalSpace;
            break;
        case CPortal::Right:
            RPHeight += portal->boundingRect().height() + m_portalSpace;
            break;
        case CPortal::Bottom:
            BPWidth += portal->boundingRect().width() + m_portalSpace;
            break;
        default:
            break;
        }
    }

	QRectF boundRect = boundingRect();

	QPointF dLP(0.0, (boundRect.height() - LPHeight + m_portalSpace)/2.0);
	QPointF dTP((boundRect.width() - TPWidth + m_portalSpace)/2.0, 0.0);
    QPointF dRP(boundRect.width() + m_portalMargin,
                (boundRect.height() - RPHeight + m_portalSpace)/2.0);
    QPointF dBP((boundRect.width() - BPWidth + m_portalSpace)/2.0,
                boundRect.height() + m_portalMargin);

    foreach (CPortal *portal, portals()) {
        if (portal == nullptr)
            continue;
        switch (portal->portalOrientation()) {
        case CPortal::Left:
            portal->setPos(QPointF(-portal->boundingRect().width() - m_portalMargin, dLP.y()));
            dLP.setY(dLP.y() + portal->boundingRect().height() + m_portalSpace);
            break;
        case CPortal::Top:
            portal->setPos(QPointF(dTP.x(), -portal->boundingRect().height() - m_portalMargin));
            dTP.setX(dTP.x() + portal->boundingRect().width() + m_portalSpace);
            break;
        case CPortal::Right:
            portal->setPos(dRP);
            dRP.setY(dRP.y() + portal->boundingRect().height() + m_portalSpace);
            break;
        case CPortal::Bottom:
            portal->setPos(dBP);
            dBP.setX(dBP.x() + portal->boundingRect().width() + m_portalSpace);
            break;
        default:
            break;
        }
	}
}

void CAlgorithm::removePortal(const QString &id)
{
    if (id.isEmpty())
        return;
    if (m_arguments.contains(id)) {
		CArgument *arg = m_arguments[id];
        if (arg != nullptr)
            arg->deleteLater();
    } else if (m_results.contains(id)) {
		CResult *res = m_results[id];
        if (res != nullptr)
            res->deleteLater();
	}
}

void CAlgorithm::addArgument(CArgument *arg)
{
    if(arg == nullptr) return;

	arg->setPortalOrientation(CPortal::Left);
	arg->setNomber(generateNomber<CArgument*, CArgument*>(m_arguments.values()));
	m_arguments[arg->id()] = arg;
	registerPortal(arg);
}

CArgument* CAlgorithm::addArgument(const QString &name)
{
	CArgument *arg = new CArgument(this);
	arg->setName(name);
	addArgument(arg);

	return arg;
}

void CAlgorithm::addResult(CResult *res)
{
    if(res == nullptr) return;

	res->setPortalOrientation(CPortal::Right);
    res->setDataColor(QColor(qRound(qrand()*255.0/RAND_MAX),
                             qRound(qrand()*255.0/RAND_MAX),
                             qRound(qrand()*255.0/RAND_MAX), 180));
	res->setNomber(generateNomber<CResult*, CResult*>(m_results.values()));
	res->createBuffer();
	m_results[res->id()] = res;
	registerPortal(res);
}

CResult* CAlgorithm::addResult(const QString &name)
{
	CResult *res = new CResult(this);
	res->setName(name);
	addResult(res);

	return res;
}

void CAlgorithm::clearResults()
{
    foreach (CResult *res, m_results) {
        if(res != nullptr) res->deleteLater();
	}
}

CAlgorithm::CAlgorithm(QGraphicsItem *parent) : CElement(parent)
{
	setObjectName(QStringLiteral("CAlgorithm"));

	m_portalSpace = 5.0;
	m_portalMargin = 2.0;

	setFlag(QGraphicsItem::ItemIsMovable, false);
}

void CAlgorithm::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	CElement::paint(painter, option, widget);

	QPen pen;
	pen.setColor(Qt::black);
    if (isSelected()) {
		pen.setWidthF(2.0);
    } else {
		pen.setWidthF(0.0);
	}

	painter->save();
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setRenderHint(QPainter::TextAntialiasing, true);
	painter->setPen(pen);
	painter->drawRect(boundingRect());
	painter->restore();
}

CResult* CAlgorithm::result(const QString &id)
{
    if (id.isEmpty())
        return nullptr;
    if (!m_results.contains(id))
        return nullptr;

	return m_results[id];
}

QList<CPortal*> CAlgorithm::portals()
{
	QList<CPortal*> prtls;
	prtls << argPortals() << resPortals();
	return prtls;
}

QList<CPortal*> CAlgorithm::argPortals()
{
	return getElements<CPortal*, CArgument*>(arguments());
}

QList<CPortal*> CAlgorithm::resPortals()
{
	return getElements<CPortal*, CResult*>(results());
}

CElement* CAlgorithm::createElement(const QString &typeID)
{
    if (typeID == CResult::staticMetaObject.className()) {
		return addResult(QString());
    } else if (typeID == CArgument::staticMetaObject.className()) {
		return addArgument(QString());
	}

    return nullptr;
}

void CAlgorithm::calc(const stTimeLine &timeLine)
{
    foreach (CArgument *arg, arguments()) {
        if (arg == nullptr)
            continue;
        if (!arg->isUsed())
            continue;
        if (!arg->isLoopBackPortal() && !arg->isBufferDataExist(timeLine.timeFrame))
            return;
	}
	proced(timeLine);
    foreach (CResult *res, results()) {
        if (res != nullptr)
            res->calc(timeLine);
	}
}

void CAlgorithm::onPortalDestroyed(QObject *objPortal)
{
    CArgument *argPortal = reinterpret_cast<CArgument*>(objPortal);
    CResult *resPortal = reinterpret_cast<CResult*>(objPortal);
    if (arguments().contains(argPortal)) {
        m_arguments.remove(m_arguments.key(argPortal));
    } else if (results().contains(resPortal)) {
        m_results.remove(m_results.key(resPortal));
	}
	updateGeometry();

    emit portalRemoved(objPortal);
}

void CAlgorithm::updateGeometry()
{
	CElement::updateGeometry();
	placePortals();
}