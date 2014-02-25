#include "calgdescmng.h"

using namespace gccore;

CAlgDescMng::CAlgDescMng()
{
}

AlgDesc CAlgDescMng::findDesc(const QString &id, bool &ok)
{
    ok = false;
    if (id.isEmpty() || !m_algDescs.contains(id))
        return AlgDesc();

    ok = true;
    return m_algDescs[id];
}

bool CAlgDescMng::contains(const AlgDesc &algDesc)
{
    return m_algDescs.contains(QString::fromStdString(algDesc.id));
}

void CAlgDescMng::addDesc(const AlgDesc &algDesc)
{
    if (contains(algDesc))
        return;

    m_algDescs[QString::fromStdString(algDesc.id)] = algDesc;
}