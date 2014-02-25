#include "cscheme.h"

#include "calg.h"
#include "calgdescmng.h"
#include "cportal.h"
#include "listutil.h"

using namespace gccore;

CScheme::CScheme(CAlgDescMng *algDescMng, const int &nomber)
{
    m_algDescMng = algDescMng;
    m_nomber = nomber;
}

CScheme::~CScheme()
{
    foreach (CAlg *alg, m_algs)
        if (alg != nullptr)
            delete alg;
}

QString CScheme::id()
{
    return QString("scheme_%1").arg(m_nomber);
}

QString CScheme::createAlgorithm(const QString &algDescID)
{
    if (m_algDescMng == nullptr)
        return QString();

    bool ok = false;
    AlgDesc algDesc = m_algDescMng->findDesc(algDescID, ok);
    if (ok)
        return createAlgorithm(algDesc);

    return QString();
}

QString CScheme::createAlgorithm(const AlgDesc &algDesc)
{
    int algNomber = generateNomber<CAlg*, CAlg*>(m_algs.values());
    CAlg *alg = new CAlg(algDesc, this, algNomber);
    m_algs[alg->id()] = alg;
    return alg->id();
}

CAlg* CScheme::findAlgorithm(const QString &algID)
{
    if (algID.isEmpty() || !m_algs.contains(algID))
        return nullptr;

    return m_algs[algID];
}

bool CScheme::linkAlgorithms(const QString &inAlgID, const QString &inAlgResultID,
                             const QString &outAlgID, const QString &outAlgArgumentID)
{
    CAlg *inAlg = findAlgorithm(inAlgID);
    if (inAlg == nullptr)
        return false;
    CAlg *outAlg = findAlgorithm(outAlgID);
    if (outAlg == nullptr)
        return false;
    CPortal *result = inAlg->findResult(inAlgResultID);
    if (result == nullptr)
        return false;
    CPortal *argument = outAlg->findArgument(outAlgArgumentID);
    if (argument == nullptr)
        return false;
    if (!result->linkToPortal(argument))
        return false;
    return true;
}