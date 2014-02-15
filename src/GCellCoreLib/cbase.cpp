#include "cbase.h"

void CBase::initTransactionProcessor()
{
    QObject *qObject = toQObject();
    if (qObject != nullptr) {
        m_transactionProcessor = new CTransactionProcessor();
        m_transactionProcessor->setObjectName(QStringLiteral("transactionProcessor"));
        m_transactionProcessor->setObject(qObject);
    }
}

void CBase::processTransaction(CTransaction *tranzaction)
{
    Q_UNUSED(tranzaction)
}

CBase::CBase()
{
    m_transactionProcessor = nullptr;
}

CBase::~CBase()
{
    if (m_transactionProcessor != nullptr)
        delete m_transactionProcessor;
}
