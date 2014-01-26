#include "celement.h"

#include <QAction>
#include <QCoreApplication>
#include <QChildEvent>
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

CGraphicsTextItem::CGraphicsTextItem(const QString &text, QGraphicsItem *parent)
    : QGraphicsTextItem(text, parent)
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
	setPlainText(text);
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
    switch(change)
    {
    case QGraphicsItem::ItemPositionChange:
        if (m_grid != nullptr) {
            QPointF newPos = value.toPointF();
            newPos = m_grid->align(newPos);
            if (m_grid->bounds() != nullptr) {
                if (m_grid->bounds()->isOutBounds(newPos, boundingRect())) {
                    newPos = pos();
                } else {
                    newPos = m_grid->bounds()->posByBound(newPos, boundingRect());
                }
            }

            if (newPos != pos())
                emit modified();

            return QVariant(newPos);
        }
        break;
    case QGraphicsItem::ItemSceneChange:
    {
        QGraphicsScene *newScene = value.value<QGraphicsScene*>();
        if ((newScene == nullptr) && (scene() != nullptr)) {
            if (parentItem() == nullptr) {
                QChildEvent childEvent(QEvent::ChildRemoved, this);
                QCoreApplication::sendEvent(scene(), &childEvent);
            }
        }
        break;
    }
    case QGraphicsItem::ItemSceneHasChanged:
    {
        QGraphicsScene *newScene = value.value<QGraphicsScene*>();
        if (newScene != nullptr) {
            if (parentItem() == nullptr) {
                QChildEvent childEvent(QEvent::ChildAdded, this);
                QCoreApplication::sendEvent(newScene, &childEvent);
            }
        }
        break;
    }
    case QGraphicsItem::ItemParentChange:
    {
        QGraphicsObject *newParent = dynamic_cast<QGraphicsObject*>(value.value<QGraphicsItem*>());
        QGraphicsObject *oldParent = dynamic_cast<QGraphicsObject*>(parentItem());
        if ((newParent == nullptr) && (oldParent != nullptr)) {
            QChildEvent childEvent(QEvent::ChildRemoved, this);
            QCoreApplication::sendEvent(oldParent, &childEvent);
        }
        break;
    }
    case QGraphicsItem::ItemParentHasChanged:
    {
        QGraphicsObject *newParent = dynamic_cast<QGraphicsObject*>(value.value<QGraphicsItem*>());
        if (newParent != nullptr) {
            QChildEvent childEvent(QEvent::ChildAdded, this);
            QCoreApplication::sendEvent(newParent, &childEvent);
        }
        break;
    }
    default:
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

	m_captionEditor = new CGraphicsTextItem(QString(), this);
	m_captionEditor->setObjectName(QStringLiteral("captionEditor"));
	connect(m_captionEditor, SIGNAL(textChanged(QString)), this, SLOT(onCaptionEditorTextChanged(QString)));

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
	emit modified();
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

QString CElement::caption()
{
    if (name().isEmpty())
        return QString("%1_%2").arg(defaultName()).arg(nomber());

	return name();
}

void CElement::addAction(QAction *action)
{
    if (!action || m_actions.contains(action))
        return;

	m_actions << action;
}

CElement* CElement::childElement(const QString &id)
{
    foreach (CElement *chel, childElements()) {
        if (chel->id() == id)
            return chel;
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
	if(m_captionEditor && !m_blockCaptionEditorSetText)
	{
		m_captionEditor->blockSignals(true);
		m_captionEditor->setText(caption());
		m_captionEditor->blockSignals(false);
	}
	m_blockCaptionEditorSetText = false;
	updateGeometry();
}

void CElement::updateGeometry(void)
{
	m_boundingRect = calcBounds();
	if(m_captionEditor) m_captionEditor->setPos(captionEditorPosition());
}
