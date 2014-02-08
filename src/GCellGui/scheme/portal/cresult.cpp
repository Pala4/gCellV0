#include "cresult.h"

#include "../link/clink.h"

QPainterPath CResult::calcPortalForm(void)
{
    QRectF m_arrowRect(0.0, 0.0, size(), size());
    QPointF p0;
    QPointF p1;
    QPointF p2;

    switch (portalOrientation()) {
    case CPortal::Left:
        p0 = m_arrowRect.topRight();
        p1 = QPointF(m_arrowRect.left(), m_arrowRect.center().y());
        p2 = m_arrowRect.bottomRight();
        break;
    case CPortal::Top:
        p0 = m_arrowRect.bottomLeft();
        p1 = QPointF(m_arrowRect.center().x(), m_arrowRect.top());
        p2 = m_arrowRect.bottomRight();
        break;
    case CPortal::Right:
        p0 = m_arrowRect.topLeft();
        p1 = QPointF(m_arrowRect.right(), m_arrowRect.center().y());
        p2 = m_arrowRect.bottomLeft();
        break;
    case CPortal::Bottom:
        p0 = m_arrowRect.topLeft();
        p1 = QPointF(m_arrowRect.center().x(), m_arrowRect.bottom());
        p2 = m_arrowRect.topRight();
        break;
    case CPortal::NoOrientation:
        break;
    default:
        break;
    }
    setLinkPos(p1);

    QPainterPath m_arrowForm = QPainterPath();
    m_arrowForm.moveTo(p0);
    m_arrowForm.lineTo(p1);
    m_arrowForm.lineTo(p2);
    m_arrowForm.closeSubpath();

    return m_arrowForm;
}

CResult::CResult(QGraphicsItem *parent) : CPortal(parent)
{
    setObjectName(QStringLiteral("CResult"));

    setPortalOrientation(CPortal::Right);
    setDefaultName(tr("res"));
}

void CResult::calc(const unsigned long long &ullTFIndex, const long double &ldblTimeFrame,
                   const long double &ldblStartTime, const long double &ldblTimeStep,
                   const long double &ldblEndTime)
{
    foreach (CLink *link, links()) {
        if (link != nullptr)
            link->calc(ullTFIndex, ldblTimeFrame, ldblStartTime, ldblTimeStep, ldblEndTime);
    }
}