#include "cbase.h"

void CBase::initCmdEventProcessor()
{
    QObject *qObject = toQObject();
    if (qObject != nullptr) {
        m_cmdEventProc = new CCmdEventProcessor();
        m_cmdEventProc->setObjectName(QStringLiteral("cmdEventProc"));
        m_cmdEventProc->setObject(qObject);
    }
}

void CBase::processCommand(CCmdEvent *event)
{
    Q_UNUSED(event)
}

CBase::CBase()
{
    m_cmdEventProc = nullptr;
}

CBase::~CBase()
{
    if (m_cmdEventProc != nullptr)
        delete m_cmdEventProc;
}