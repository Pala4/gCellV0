#include "cgrelement.h"

#include <QFont>
#include <QFontMetrics>
#include <QPainter>
#include <QAction>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>

#include "../../core/scheme/celement.h"

void CGrElement::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    menu.addActions(m_actions);
    menu.exec(event->screenPos());
    event->setAccepted(true);
}

CGrElement::CGrElement(CElement *element, QGraphicsItem *parent) : QGraphicsObject(parent)
{
    setObjectName(QStringLiteral("CGrElement"));

    m_acDelete = 0;
    m_element = 0;

    m_acDelete = new QAction(tr("Delete"), this);
    addAction(m_acDelete);

    setFlag(QGraphicsItem::ItemIsSelectable);

	setElement(element);
}

QRectF CGrElement::boundingRect(void) const
{
	if(!m_element) return QRectF();

	QFont f("Corier", 8, QFont::Bold);
	QFontMetricsF fm(f);
	return fm.boundingRect(m_element->name());
}

void CGrElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)

	painter->save();
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setRenderHint(QPainter::TextAntialiasing, true);

	painter->drawRect(boundingRect());
	if(m_element) painter->drawText(boundingRect(), m_element->name());

    painter->restore();
}

void CGrElement::addAction(QAction *action)
{
    if(!action) return;
    if(m_actions.contains(action)) return;

    m_actions << action;
}

void CGrElement::setElement(CElement *element)
{
	if(m_element && (m_element == element)) return;

	if(m_element)
	{
		disconnect(m_element, SIGNAL(nameChanged(QString)), this, SLOT(onElementStateChanged()));
        disconnect(m_element, SIGNAL(destroyed()), this, SLOT(deleteLater()));
        if(m_acDelete) disconnect(m_acDelete, SIGNAL(triggered()), m_element, SLOT(deleteLater()));
	}

	m_element = element;
	setPos(m_element->pos());

	if(m_element)
	{
		connect(m_element, SIGNAL(nameChanged(QString)), this, SLOT(onElementStateChanged()));
        connect(m_element, SIGNAL(destroyed()), this, SLOT(deleteLater()));
        if(m_acDelete) connect(m_acDelete, SIGNAL(triggered()), m_element, SLOT(deleteLater()));
    }
}

void CGrElement::onElementStateChanged(void)
{
    update();
}