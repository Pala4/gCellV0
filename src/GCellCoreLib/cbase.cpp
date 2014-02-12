#include "cbase.h"

void CBase::initCommandDsp()
{
    QObject *qObject = toQObject();
    if (qObject != nullptr) {
        m_commandDsp = new CCommandDsp();
        m_commandDsp->setObjectName(QStringLiteral("commandDsp"));
        m_commandDsp->setObject(qObject);
    }
}

void CBase::processCommand(QEvent *event)
{
    Q_UNUSED(event)
}

CBase::CBase()
{
    m_commandDsp = nullptr;
}

CBase::~CBase()
{
    if (m_commandDsp != nullptr)
        delete m_commandDsp;
}