#include "celement.h"

#include <QAction>
#include <QPainter>
#include <QTextCursor>
#include <QTextDocument>
#include <QGraphicsSceneMouseEvent>

#include "cscheme.h"
#include "cgrid.h"
#include "cbounds.h"
#include "elementlistutil.h"
#include "celementoptionswgt.h"

/*!
 * \class CGraphicsTextItem
 */
QVariant CGraphicsTextItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
	switch((int)change)
	{
		case QGraphicsItem::ItemSelectedHasChanged:
			if(!isSelected())
			{
				setTextInteractionFlags(Qt::NoTextInteraction);
				QTextCursor txtCursor = textCursor();
				txtCursor.clearSelection();
				setTextCursor(txtCursor);
				setFlag(QGraphicsItem::ItemStacksBehindParent, true);
			}
			else
			{
				if(textInteractionFlags() == Qt::NoTextInteraction)
				{
					setTextInteractionFlags(Qt::TextEditorInteraction);
				}
			}
		break;
	}
	return QGraphicsTextItem::itemChange(change, value);
}

CGraphicsTextItem::CGraphicsTextItem(const QString &text, QGraphicsItem *parent) : QGraphicsTextItem(text, parent)
{
	setObjectName("CGraphicsTextItem");

	connect(document(), SIGNAL(contentsChanged()), this, SLOT(onDocumentContentChanged()));

	setFlag(QGraphicsItem::ItemIsMovable, false);
	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemStacksBehindParent);
	setTextInteractionFlags(Qt::NoTextInteraction);
}

void CGraphicsTextItem::onDocumentContentChanged(void)
{
	emit textChanged(toPlainText());
}

void CGraphicsTextItem::setText(const QString &text)
{
<<<<<<< HEAD
<<<<<<< HEAD
	setPlainText(text);
=======
=======
>>>>>>> a1b3f10986557e6b9f1c4305267c0248c98516c9
	QString oldText = toPlainText();

	setPlainText(text);

	if(oldText != text) emit textChanged(text);
<<<<<<< HEAD
>>>>>>> a1b3f10986557e6b9f1c4305267c0248c98516c9
=======
>>>>>>> a1b3f10986557e6b9f1c4305267c0248c98516c9
}

/*!
 * \class CElement
 */
CScheme* CElement::scheme(void)
{
	return qobject_cast<CScheme*>(scene());
}

QRectF CElement::calcBounds(void)
{
	QRectF r = m_captionEditor->boundingRect();
	return m_captionEditor ? r : QRectF();
}

void CElement::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsObject::mouseDoubleClickEvent(event);
	if(m_captionEditor) m_captionEditor->setFlag(QGraphicsItem::ItemStacksBehindParent, false);
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

QVariant CElement::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
	switch((int)change)
	{
		case QGraphicsItem::ItemPositionChange:
			if(m_grid)
			{
				QPointF newPos = value.toPointF();
				newPos = m_grid->align(newPos);
				if(m_grid->bounds())
				{
					if(m_grid->bounds()->isOutBounds(newPos, boundingRect()))
					{
						newPos = pos();
					}
					else
					{
						newPos = m_grid->bounds()->posByBound(newPos, boundingRect());
					}
				}
				return QVariant(newPos);
			}
		break;
	}

	return QGraphicsObject::itemChange(change, value);
}

CElement::CElement(QGraphicsItem *parent) : QGraphicsObject(parent)
{
	setObjectName(QStringLiteral("CElement"));

	m_defaultName = tr("element");
	m_nomber = 0;
	m_blockCaptionEditorSetText = false;
	m_captionEditor = 0;
	m_grid = 0;
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> a1b3f10986557e6b9f1c4305267c0248c98516c9

	m_captionEditor = new CGraphicsTextItem(QString(), this);
	m_captionEditor->setObjectName(QStringLiteral("captionEditor"));
	connect(m_captionEditor, SIGNAL(textChanged(QString)), this, SLOT(onCaptionEditorTextChanged(QString)));

<<<<<<< HEAD
=======

	m_captionEditor = new CGraphicsTextItem(QString(), this);
	m_captionEditor->setObjectName(QStringLiteral("captionEditor"));
	connect(m_captionEditor, SIGNAL(textChanged(QString)), this, SLOT(onCaptionEditorTextChanged(QString)));

>>>>>>> a1b3f10986557e6b9f1c4305267c0248c98516c9
=======
>>>>>>> a1b3f10986557e6b9f1c4305267c0248c98516c9
	connect(this, SIGNAL(nameChanged(QString)), this, SLOT(onNameChanged(QString)));

	setInteractions(CElement::AllIntercations);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
	setZValue(1000.0);
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

void CElement::onCaptionEditorTextChanged(const QString &text)
{
	if(name() == text) return;
	m_blockCaptionEditorSetText = true;
	setName(text);
}

void CElement::onNameChanged(const QString &name)
{
	Q_UNUSED(name)
<<<<<<< HEAD
<<<<<<< HEAD
	if(m_captionEditor && !m_blockCaptionEditorSetText)
	{
		m_captionEditor->blockSignals(true);
		m_captionEditor->setText(caption());
		m_captionEditor->blockSignals(false);
	}
=======
	if(m_captionEditor && !m_blockCaptionEditorSetText) m_captionEditor->setText(caption());
>>>>>>> a1b3f10986557e6b9f1c4305267c0248c98516c9
=======
	if(m_captionEditor && !m_blockCaptionEditorSetText) m_captionEditor->setText(caption());
>>>>>>> a1b3f10986557e6b9f1c4305267c0248c98516c9
	m_blockCaptionEditorSetText = false;
	updateGeometry();
}

void CElement::updateGeometry(void)
{
	m_boundingRect = calcBounds();
	if(m_captionEditor) m_captionEditor->setPos(captionEditorPosition());
<<<<<<< HEAD
<<<<<<< HEAD

	emit modified();
=======
>>>>>>> a1b3f10986557e6b9f1c4305267c0248c98516c9
=======
>>>>>>> a1b3f10986557e6b9f1c4305267c0248c98516c9
}