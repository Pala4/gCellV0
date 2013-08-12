#include "celement.h"

#include <QAction>

#include "cscheme.h"
#include "elementlistutil.h"
#include "celementoptionswgt.h"

CScheme* CElement::scheme(void)
{
	return qobject_cast<CScheme*>(scene());
}

CElement::CElement(QGraphicsItem *parent) : QGraphicsObject(parent)
{
	setObjectName(QStringLiteral("CElement"));

	m_defaultName = tr("element");
	m_nomber = 0;

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
//		if(scheme()) schemeID = scheme()->id() + ":";
	}

	return QString("%1%2%3:%4").arg(schemeID).arg(parentID).arg(typeID()).arg(nomber());
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

QString CElement::caption(void) const
{
	if(name().isEmpty()) return QString("%1_%2").arg(defaultName()).arg(nomber());
	return name();
}

void CElement::addAction(QAction *action)
{
	if(!action || m_actions.contains(action)) return;

	m_actions << action;
}

CElement* CElement::childElement(const QString &id)
{
	foreach(CElement *chel, childElements())
	{
		if(chel->id() == id) return chel;
	}
	return 0;
}

QList<CElement*> CElement::childElements(void)
{
	return getElements<CElement*, QGraphicsItem*>(childItems());
}

CElementOptionsWgt* CElement::optionsWidget(QWidget *parentWidget)
{
	return new CElementOptionsWgt(this, parentWidget);
}

void CElement::acceptOptions(CElementOptionsWgt *optWgt)
{
	if(optWgt)
	{
		setName(optWgt->elementCaption());
	}
}