#include "cbase.h"

#include <QCoreApplication>

#include "cresponsevent.h"

void CBase::initCmdEventProcessor()
{
    QObject *qObject = toQObject();
    if (qObject != nullptr) {
        m_cmdEventProc = new CQueryEventProcessor();
        m_cmdEventProc->setObjectName(QStringLiteral("cmdEventProc"));
        m_cmdEventProc->setObject(qObject);
    }
}

void CBase::processCommand(CQueryEvent *event)
{
    Q_UNUSED(event)
}

void CBase::sendRespons(QueryDesc queryDesc, const QString &respons)
{
    if (queryDesc.responsReceiver != nullptr) {
        queryDesc.responsMsg = respons;
        CResponsEvent responsEvent(queryDesc);
        QCoreApplication::postEvent(queryDesc.responsReceiver, &responsEvent);
    }
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