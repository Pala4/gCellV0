#include "celement.h"

CElement::CElement(QObject *parent) : QObject(parent)
{
	setObjectName(QStringLiteral("CElement"));

	m_nomber = 0;
	m_defaultName = "element";

	resetName();
}

QString CElement::id(void)
{
	return QString("%1:%2").arg(m_typeID).arg(m_nomber);
}

void CElement::resetName(void)
{
	setName(QString("%1_%2").arg(m_defaultName).arg(m_nomber));
}