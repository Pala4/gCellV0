#include "celementprotomng.h"

#include "celementproto.h"

CElementProtoMng::CElementProtoMng(QObject *parent) : QObject(parent)
{
    setObjectName(QStringLiteral("CElementProtoMng"));

	m_selectedElementProto = 0;
}

CElementProto* CElementProtoMng::addProto(const QString &id, const QString &name, const QMetaObject *elementMO)
{
    if(m_elementProtos.contains(id)) return 0;
    if(!CElementProto::checkElementMO(elementMO)) return 0;

    CElementProto *elementProto = new CElementProto(id, name, elementMO, this);
    m_elementProtos[id] = elementProto;

    emit elementProtoAdded(elementProto);

	return elementProto;
}

void CElementProtoMng::removeProto(const QString &id)
{
	if(!m_elementProtos.contains(id)) return;

	CElementProto *elementProto = m_elementProtos.take(id);
	emit elementProtoRemoved(elementProto);
	elementProto->deleteLater();
}

void CElementProtoMng::selectElementProto(const QString &id)
{
	if(!m_elementProtos.contains(id)) return;

	setSelectedElementProto(m_elementProtos[id]);
}

void CElementProtoMng::setSelectedElementProto(CElementProto *elementProto)
{
	if(m_selectedElementProto == elementProto) return;
	if(elementProto && (!m_elementProtos.values().contains(elementProto))) return;
	m_selectedElementProto = elementProto;
    emit elementProtoSelected(m_selectedElementProto);
}