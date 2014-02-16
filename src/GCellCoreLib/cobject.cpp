#include "cobject.h"

void CObject::initTransactionProcessor()
{
    QObject *qObject = toQObject();
    if (qObject != nullptr) {
        m_transactionProcessor = new CTransactionProcessor();
        m_transactionProcessor->setObjectName(QStringLiteral("transactionProcessor"));
        m_transactionProcessor->setObject(qObject);
    }
}

void CObject::processTransaction(CTransaction *tranzaction)
{
    Q_UNUSED(tranzaction)
}

CObject::CObject()
{
    m_transactionProcessor = nullptr;
}

CObject::~CObject()
{
    if (m_transactionProcessor != nullptr)
        delete m_transactionProcessor;
}
