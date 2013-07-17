#include "celementproto.h"

#include "../../core/scheme/celement.h"

CElementProto::CElementProto(QObject *parent) : QObject(parent)
{
    setObjectName(QStringLiteral("CElementProto"));

    m_id = QString();
    m_name = tr("Element prototype");
    m_elementMO = 0;
}

CElementProto::CElementProto(const QString &id, const QString &name, const QMetaObject *elementMO, QObject *parent) : QObject(parent)
{
    setObjectName(QStringLiteral("CElementProto"));

    m_id = id;
    m_name = name;
    m_elementMO = elementMO;
}

bool CElementProto::checkElementMO(const QMetaObject *elementMO)
{
    if(!elementMO) return false;
    if(elementMO == &CElement::staticMetaObject) return true;
    return checkElementMO(elementMO->superClass());
}

CElement* CElementProto::createElement(void)
{
    if(!checkElementMO(m_elementMO)) return 0;

    CElement *element = qobject_cast<CElement*>(m_elementMO->newInstance());
	if(element)
	{
		element->setTypeID(m_id);
		element->setDefaultName(m_name);
		element->setName(m_name);
		element->resetName();
	}

    return element;
}