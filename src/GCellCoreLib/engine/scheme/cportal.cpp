#include "cportal.h"

#include "calg.h"

using namespace gccore;

CPortal::CPortal(const PortalDesc &portalDesc, CAlg *alg, const int &nomber)
{
    m_portalDesc = portalDesc;
    m_alg = alg;
    m_nomber = nomber;
}

bool CPortal::linkToPortal(CPortal *portal)
{
    if (portal == nullptr)
        return false;
    if (m_linkedPortals.values().contains(portal))
        return false;

    m_linkedPortals[portal->id()] = portal;
    return true;
}

QString CPortal::id()
{
    QString algID = (m_alg != nullptr) ? m_alg->id() + ":" : QString();
    QString portalID = (m_portalDesc.portalType == PortalDesc::Argument) ? QStringLiteral("Arg:")
                                                                         : QStringLiteral("Res:");
    return QString("%1%2%3").arg(algID).arg(portalID).arg(m_nomber);
}