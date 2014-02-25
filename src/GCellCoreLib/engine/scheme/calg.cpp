#include "calg.h"

#include "cscheme.h"
#include "cportal.h"
#include "listutil.h"

using namespace gccore;

CAlg::CAlg(const AlgDesc &algDesc, CScheme *scheme, const int &nomber)
{
    m_nomber = nomber;
    m_algDesc = algDesc;
    m_scheme = scheme;
}

CAlg::~CAlg()
{
    clearArguments();
    clearResults();
}

void CAlg::setAlgDesc(const AlgDesc &algDesc)
{
    clearArguments();
    clearResults();
    m_algDesc = algDesc;
    createPortals(m_algDesc.arguments);
    createPortals(m_algDesc.results);
}

void CAlg::createPortals(const std::map<std::string, PortalDesc> &portalDescs)
{
    for (std::pair<std::string, PortalDesc> portalDesc : portalDescs) {
        if (portalDesc.second.portalType == PortalDesc::Argument) {
            int portalNomber = generateNomber<CPortal*, CPortal*>(m_arguments.values());
            CPortal *portal = new CPortal(portalDesc.second, this, portalNomber);
            m_arguments[portal->id()] = portal;
        } else if (portalDesc.second.portalType == PortalDesc::Result) {
            int portalNomber = generateNomber<CPortal*, CPortal*>(m_results.values());
            CPortal *portal = new CPortal(portalDesc.second, this, portalNomber);
            m_results[portal->id()] = portal;
        }
    }
}

CPortal* CAlg::findArgument(const QString &argumentID)
{
    if (argumentID.isEmpty() || !m_arguments.contains(argumentID))
        return nullptr;

    return m_arguments[argumentID];
}

CPortal* CAlg::findResult(const QString &resultID)
{
    if (resultID.isEmpty() || !m_results.contains(resultID))
        return nullptr;

    return m_results[resultID];
}

void CAlg::clearArguments()
{
    foreach (CPortal *portal, m_arguments)
        if (portal != nullptr)
            delete portal;

    m_arguments.clear();
}

void CAlg::clearResults()
{
    foreach (CPortal *portal, m_results)
        if (portal != nullptr)
            delete portal;

    m_results.clear();
}

QString CAlg::id()
{
    QString schemeID = (m_scheme != nullptr) ? m_scheme->id() + QLatin1Char(':') : QString();
    return QString(QStringLiteral("%1%2:%3")).arg(schemeID)
            .arg(QString::fromStdString(m_algDesc.id)).arg(m_nomber);
}