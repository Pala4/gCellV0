#include "calgorithmproto.h"

#include "../algorithm/calgorithm.h"

CAlgorithmProto::CAlgorithmProto(QObject *parent) : QObject(parent)
{
	setObjectName(QStringLiteral("CAlgorithmProto"));

	m_name = tr("Algorithm prototype");
	m_algorithmMO = 0;
}

CAlgorithmProto::CAlgorithmProto(const QString &name, const QString &groupName, const QMetaObject *algorithmMO, QObject *parent) : QObject(parent)
{
	setObjectName(QStringLiteral("CAlgorithmProto"));

    m_name = name;
	m_groupName = groupName;
	m_algorithmMO = algorithmMO;
}

bool CAlgorithmProto::checkAlgorithmMO(const QMetaObject *algorithmMO)
{
	if(!algorithmMO) return false;
	if(algorithmMO == &CAlgorithm::staticMetaObject) return true;
	return checkAlgorithmMO(algorithmMO->superClass());
}

CAlgorithm* CAlgorithmProto::createAlgorithm(void)
{
	if(!checkAlgorithmMO(m_algorithmMO)) return 0;

	CAlgorithm *algorithm = qobject_cast<CAlgorithm*>(m_algorithmMO->newInstance());
	if(algorithm)
	{
		algorithm->setDefaultName(m_name);
	}
	return algorithm;
}