#include "cengine.h"

#include "../scheme/cscheme.h"

CEngine::CEngine(QObject *parent) : QObject(parent)
{
    setObjectName(QStringLiteral("CEngine"));

    m_scheme = 0;
}

void CEngine::setScheme(CScheme *scheme)
{
    if(m_scheme && (m_scheme == scheme)) return;

    if(m_scheme)
    {
        disconnect(m_scheme, SIGNAL(destroyed()), this, SLOT(onSchemeDestroyed()));
    }

    m_scheme = scheme;

    if(m_scheme)
    {
        connect(m_scheme, SIGNAL(destroyed()), this, SLOT(onSchemeDestroyed()));
    }
}

void CEngine::onSchemeDestroyed(void)
{
    m_scheme = 0;
}

void CEngine::calc(void)
{
}