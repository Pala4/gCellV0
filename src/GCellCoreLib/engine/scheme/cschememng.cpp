#include "cschememng.h"

#include "calgdescmng.h"
#include "cscheme.h"
#include "cengine.h"
#include "listutil.h"

using namespace gccore;

CSchemeMng::CSchemeMng(QObject *parent) : QObject(parent)
{
    setObjectName(QStringLiteral("CWorkSpace"));

    m_algDescMng = new CAlgDescMng();
    m_engine = new CEngine();
}

CSchemeMng::~CSchemeMng()
{
    if (m_algDescMng != nullptr)
        delete m_algDescMng;
    foreach (CScheme *scheme, m_schemes)
        if (scheme != nullptr)
            delete scheme;
    if (m_engine != nullptr)
        delete m_engine;
}

CScheme* CSchemeMng::findScheme(const QString &schemeID)
{
    if (schemeID.isEmpty() || !m_schemes.contains(schemeID))
        return nullptr;

    return m_schemes[schemeID];
}

QString CSchemeMng::createScheme()
{
    int schemeNomber = generateNomber<CScheme*, CScheme*>(m_schemes.values());
    CScheme *scheme = new CScheme(m_algDescMng, schemeNomber);
    m_schemes[scheme->id()] = scheme;
    return scheme->id();
}

QString CSchemeMng::createAlgorithm(const QString &schemeID, const QString &algID)
{
    CScheme *scheme = findScheme(schemeID);
    if (scheme == nullptr)
        return QString();

    return scheme->createAlgorithm(algID);
}

void CSchemeMng::linkAlgorithms(const QString &schemeID,
                                const QString &inAlgID, const QString &inAlgResultID,
                                const QString &outAlgID, const QString &outAlgArgumentID)
{
    CScheme *scheme = findScheme(schemeID);
    if (scheme == nullptr)
        return;

    scheme->linkAlgorithms(inAlgID, inAlgResultID, outAlgID, outAlgArgumentID);
}

void CSchemeMng::calcScheme(const QString &schemeID)
{
    if (m_engine == nullptr)
        return;

    CScheme *scheme = findScheme(schemeID);
    if (scheme == nullptr)
        return;

    m_engine->calcScheme(scheme);
}