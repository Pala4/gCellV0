#include "cscheme.h"

#include "celement.h"

CScheme::CScheme(QObject *parent) : QObject(parent)
{
    setObjectName(QStringLiteral("CScheme"));
}

void CScheme::addElement(CElement *element)
{
	if(!element) return;
	if(m_elements.contains(element)) return;

	m_elements << element;
	emit elementAdded(element);
}

void CScheme::addLink(CElement *inElement, CElement *outElement)
{
    Q_UNUSED(inElement)
    Q_UNUSED(outElement)
}