#include "celement.h"

#include <QAction>

#include "cscheme.h"

CScheme* CElement::scheme(void)
{
	return qobject_cast<CScheme*>(scene());
}

CElement::CElement(QGraphicsItem *parent) : QGraphicsObject(parent)
{
	setObjectName(QStringLiteral("CElement"));

	m_defaultName = tr("element");
	m_nomber = 0;

	QAction *acDelete = new QAction("Delete", this);
	connect(acDelete, SIGNAL(triggered()), this, SLOT(deleteLater()));
	addAction(acDelete);

	connect(this, SIGNAL(nameChanged(QString)), this, SLOT(updateGeometry()));
}

QString CElement::id(void)
{
	QString schemeID;
	QString parentID;
	CElement *parentElement = dynamic_cast<CElement*>(parentItem());
	if(parentElement)
	{
		parentID = parentElement->id() + ":";
	}
	else
	{
		if(scheme()) schemeID = scheme()->id() + ":";
	}

	return QString("%1%2%3:%4").arg(schemeID).arg(parentID).arg(typeID()).arg(nomber());
}

QString CElement::name(void) const
{
	if(m_name.isEmpty()) return QString("%1_%2").arg(m_defaultName).arg(m_nomber);
	return m_name;
}

void CElement::setName(const QString &name)
{
	if(m_name == name) return;
	m_name = name;
	emit nameChanged(m_name);
}

void CElement::setDefaultName(const QString &defaultName)
{
	m_defaultName = defaultName;
	if(m_name.isEmpty()) emit nameChanged(m_defaultName);
}

void CElement::addAction(QAction *action)
{
	if(!action || m_actions.contains(action)) return;

	m_actions << action;
}