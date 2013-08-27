#include "calgorithmprotomng.h"

#include "calgorithmproto.h"

CAlgorithmProtoMng::CAlgorithmProtoMng(QObject *parent) : QObject(parent)
{
	setObjectName(QStringLiteral("CAlgorithmProtoMng"));

	m_selectedAlgorithmProto = 0;
}

CAlgorithmProto* CAlgorithmProtoMng::algorithmProto(const QString &id)
{
    foreach(CAlgorithmProto *algProto, m_algorithmProtos)
    {
        if(algProto && algProto->id() == id) return algProto;
    }

    return 0;
}

CAlgorithmProto* CAlgorithmProtoMng::addProto(const QString &name, const QString &groupName,
											  const QMetaObject *algorithmMO)
{
    if(algorithmProto(algorithmMO->className())) return 0;
	if(!CAlgorithmProto::checkAlgorithmMO(algorithmMO)) return 0;

	CAlgorithmProto *algorithmProto = new CAlgorithmProto(name, groupName, algorithmMO, this);
    m_algorithmProtos << algorithmProto;

	emit algorithmProtoAdded(algorithmProto);

	return algorithmProto;
}

void CAlgorithmProtoMng::setSelectedAlgorithmProto(CAlgorithmProto *algorithmProto)
{
	if(m_selectedAlgorithmProto == algorithmProto) return;
    if(algorithmProto && (!m_algorithmProtos.contains(algorithmProto))) return;
	m_selectedAlgorithmProto = algorithmProto;
	emit algorithmProtoSelected(m_selectedAlgorithmProto);
}
