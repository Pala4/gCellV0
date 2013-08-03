#include "calgorithmprotomng.h"

#include "calgorithmproto.h"

CAlgorithmProtoMng::CAlgorithmProtoMng(QObject *parent) : QObject(parent)
{
	setObjectName(QStringLiteral("CAlgorithmProtoMng"));

	m_selectedAlgorithmProto = 0;
}

CAlgorithmProto* CAlgorithmProtoMng::algorithmProto(const QString &id)
{
	if(!m_algorithmProtos.contains(id)) return 0;
	return m_algorithmProtos[id];
}

CAlgorithmProto* CAlgorithmProtoMng::addProto(const QString &name, const QMetaObject *algorithmMO)
{
	if(m_algorithmProtos.contains(algorithmMO->className())) return 0;
	if(!CAlgorithmProto::checkAlgorithmMO(algorithmMO)) return 0;

	CAlgorithmProto *algorithmProto = new CAlgorithmProto(name, algorithmMO, this);
	m_algorithmProtos[algorithmMO->className()] = algorithmProto;

	emit algorithmProtoAdded(algorithmProto);

	return algorithmProto;
}

void CAlgorithmProtoMng::setSelectedAlgorithmProto(CAlgorithmProto *algorithmProto)
{
	if(m_selectedAlgorithmProto == algorithmProto) return;
	if(algorithmProto && (!m_algorithmProtos.values().contains(algorithmProto))) return;
	m_selectedAlgorithmProto = algorithmProto;
	emit algorithmProtoSelected(m_selectedAlgorithmProto);
}