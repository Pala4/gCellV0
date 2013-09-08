#include "celement.h"

#include <QAction>
#include <QPainter>

#include "cscheme.h"
#include "elementlistutil.h"
#include "celementoptionswgt.h"

CScheme* CElement::scheme(void)
{
	return qobject_cast<CScheme*>(scene());
}

void CElement::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	update();
	QGraphicsObject::mousePressEvent(event);
}

void CElement::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
//	update();
	QGraphicsObject::mouseMoveEvent(event);
}

void CElement::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	update();
	QGraphicsObject::mouseReleaseEvent(event);
}

CElement::CElement(QGraphicsItem *parent) : QGraphicsObject(parent)
{
	setObjectName(QStringLiteral("CElement"));

	m_defaultName = tr("element");
	m_nomber = 0;
	m_captionVisible = true;

	connect(this, SIGNAL(nameChanged(QString)), this, SLOT(updateGeometry()));

	setInteractions(CElement::AllIntercations);
	setFlag(QGraphicsItem::ItemIsSelectable);
}

void CElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)
	painter->setFont(captionFont());
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

void CElement::setCaptionFont(const QFont &captionFont)
{
	if(m_captionFont == captionFont) return;
	m_captionFont = captionFont;
	updateGeometry();
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
	CElementOptionsWgt *optWgt = new CElementOptionsWgt(this, parentWidget);
	optWgt->setCaptionVisible(isCaptionVisible());
	return optWgt;
}

void CElement::acceptOptions(CElementOptionsWgt *optWgt)
{
	if(optWgt)
	{
		setName(optWgt->elementCaption());
		setCaptionVisible(optWgt->isCaptionVisible());
	}
}

void CElement::beforeCalc(const qreal &startTime, const qreal &timeStep, const qreal &endTime)
{
	Q_UNUSED(startTime)
	Q_UNUSED(timeStep)
	Q_UNUSED(endTime)
}