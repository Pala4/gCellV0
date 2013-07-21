#include "cschemeeditor.h"

#include <QMenu>
#include <QContextMenuEvent>

#include "cscheme.h"
#include "calgorithmproto.h"
#include "cportal.h"

/*!
 * \class CSelector
 */
CSelector::CSelector(void)
{
	m_started = false;
	m_selectorRect = QRectF();
}

void CSelector::start(const QPointF &pos)
{
	m_started = true;
	m_selectorRect = QRectF(pos, pos);
}

void CSelector::move(const QPointF &pos)
{
	if(!m_started) return;
	m_selectorRect.setBottomLeft(pos);
}

QRectF CSelector::release(const QPointF &pos)
{
	m_started = false;
	m_selectorRect.setBottomLeft(pos);
	QRectF selRect = m_selectorRect;
	m_selectorRect = QRectF();
	return selRect;
}

void CSelector::draw(QPainter *painter)
{
	if(!painter) return;
	if(!m_started) return;

	painter->save();
	QPen pen;
	pen.setColor(Qt::black);
	painter->setPen(pen);
	painter->drawRect(m_selectorRect);
	painter->restore();
}

/*!
 * \class CSchemeView
 */
CScheme* CSchemeEditor::scheme(void)
{
	return qobject_cast<CScheme*>(scene());
}

void CSchemeEditor::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu menu;

	CElement *element = 0;
	QGraphicsItem *item = itemAt(event->pos());
	if(item && (!item->isSelected() || (item->isSelected() && scene() && (scene()->selectedItems().count() == 1))))
	{
		element = dynamic_cast<CElement*>(item);
	}
	if(element)
	{
		menu.addActions(element->actions());
	}
	else
	{
		menu.addActions(schemeEditorActions());
	}

	menu.exec(event->globalPos());
}

void CSchemeEditor::mousePressEvent(QMouseEvent *event)
{
	if(!(event->buttons() & Qt::RightButton)) QGraphicsView::mousePressEvent(event);

	switch((int)m_mouseMode)
	{
		case CSchemeEditor::MoveSelectMode:
		{
			if(event->buttons() & Qt::LeftButton)
			{
				if(dynamic_cast<CAlgorithm*>(itemAt(event->pos())))
				{
					m_algorithmMover.release();
					if(scene()) m_algorithmMover.addElements(scene()->selectedItems(), mapToScene(event->pos()));
				}
				else
				{
					m_selector.start(mapToScene(event->pos()));
					if(scene()) scene()->update();
				}
			}
		}
		break;
		case CSchemeEditor::MoveSceneMode:break;
		case CSchemeEditor::LinkingMode:
		{
			CPortal *portal = dynamic_cast<CPortal*>(itemAt(event->pos()));
			if(portal)
			{
				if(!m_firstPortal)
				{
					m_firstPortal = portal;
				}
				else if(!m_secondPortal)
				{
					m_secondPortal = portal;
				}
			}
		}
		break;
		case CSchemeEditor::AddAlgorithmMode:
		{
			if(event->buttons() & Qt::LeftButton)
			{
				if(m_currentProto && scheme())
				{
					scheme()->addAlgorithm(m_currentProto, mapToScene(event->pos()));
				}
			}
		}
		break;
	}
}

void CSchemeEditor::mouseMoveEvent(QMouseEvent *event)
{
	switch((int)m_mouseMode)
	{
		case CSchemeEditor::MoveSelectMode:
		{
			if(event->buttons() & Qt::LeftButton)
			{
				if(m_algorithmMover.haveMoved())
				{
					m_algorithmMover.move(mapToScene(event->pos()));
				}
				else
				{
					if(m_selector.isStarted())
					{
						m_selector.move(mapToScene(event->pos()));
						if(scene()) scene()->update();
					}
				}
			}
			break;
		}
		case CSchemeEditor::LinkingMode:break;
	}
}

void CSchemeEditor::mouseReleaseEvent(QMouseEvent *event)
{
	QGraphicsView::mouseReleaseEvent(event);

	m_algorithmMover.release(mapToScene(event->pos()));
	if(m_selector.isStarted())
	{
		QRectF selRect = m_selector.release(mapToScene(event->pos()));
		if(scene())
		{
			QPainterPath path;
			path.addRect(selRect);
			scene()->setSelectionArea(path);
		}
		if(scene()) scene()->update();
	}
	if(m_firstPortal && m_secondPortal)
	{
		if(scheme()) scheme()->addLink(m_firstPortal, m_secondPortal);
		m_firstPortal = 0;
		m_secondPortal = 0;
	}
	emit mouseReleased(mapToScene(event->pos()));
}

void CSchemeEditor::drawForeground(QPainter *painter, const QRectF &rect)
{
	Q_UNUSED(rect)
	if(m_selector.isStarted())
	{
		m_selector.draw(painter);
	}
}

CSchemeEditor::CSchemeEditor(QWidget *parent) : QGraphicsView(parent)
{
	setObjectName(QStringLiteral("CSchemeView"));

	m_currentProto = 0;
	m_mouseMode = CSchemeEditor::MoveSelectMode;
	m_firstPortal = 0;
	m_secondPortal = 0;
}

QList<QAction*> CSchemeEditor::schemeEditorActions(void)
{
	QList<QAction*> seActions = actions();
	if(scheme() && !scheme()->actions().isEmpty())
	{
		QAction *sep = new QAction("-", this);
		sep->setSeparator(true);
		seActions << sep;
		seActions << scheme()->actions();
	}
	return seActions;
}

void CSchemeEditor::setCurrentProto(CAlgorithmProto *currentProto)
{
	if(m_currentProto && (m_currentProto == currentProto)) return;

	if(m_currentProto)
	{
		disconnect(m_currentProto, SIGNAL(destroyed()), this, SLOT(onCurrentProtoDestroyed()));
	}
	m_currentProto = currentProto;
	if(m_currentProto)
	{
		connect(m_currentProto, SIGNAL(destroyed()), this, SLOT(onCurrentProtoDestroyed()));
	}
}

void CSchemeEditor::deleteSelected(void)
{
	if(scheme()) scheme()->deleteSelected();
}