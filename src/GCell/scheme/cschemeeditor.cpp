#include "cschemeeditor.h"

#include <QMenu>
#include <QContextMenuEvent>
#include <QApplication>
#include <QMimeData>

#include "cscheme.h"
#include "portal/cportal.h"

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
		if(!element->actions().isEmpty()) menu.addSeparator();
	}

	menu.addActions(actions());

	menu.exec(event->globalPos());
}

void CSchemeEditor::mousePressEvent(QMouseEvent *event)
{
	if((event->buttons() & Qt::RightButton))
	{
		if(itemAt(event->pos()))
		{
			if(scheme()) scheme()->clearSelection();
			itemAt(event->pos())->setSelected(true);
		}
		return;
	}
	else
	{
		QGraphicsView::mousePressEvent(event);
	}

	switch((int)m_mouseMode)
	{
		case CSchemeEditor::MoveSelectMode:
		{
			if(event->buttons() & Qt::LeftButton)
			{
				if(itemAt(event->pos()))
				{
					if(dynamic_cast<CAlgorithm*>(itemAt(event->pos())))
					{
						m_algorithmMover.release();
						if(scene()) m_algorithmMover.addElements(scene()->selectedItems(), mapToScene(event->pos()));
					}
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
			if(event->buttons() & Qt::LeftButton)
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
					portal->setChecked(true);
				}
				else
				{
					if(m_firstPortal)
					{
						m_firstPortal->setChecked(false);
						m_firstPortal = 0;
					}
					if(m_secondPortal)
					{
						m_secondPortal->setChecked(false);
						m_secondPortal = 0;
					}
				}
			}
		}
		break;
		case CSchemeEditor::AddAlgorithmMode:
		{
			if(event->buttons() & Qt::LeftButton)
			{
                if(scheme()) scheme()->createAlgorithm(mapToScene(event->pos()));
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

	QGraphicsView::mouseMoveEvent(event);
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
		m_firstPortal->setChecked(false);
		m_firstPortal = 0;
		m_secondPortal->setChecked(false);
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

	m_acCopy = 0;
	m_acPaste = 0;
	m_acCut = 0;
	m_acDelete = 0;
	m_mouseMode = CSchemeEditor::MoveSelectMode;
	m_firstPortal = 0;
	m_secondPortal = 0;

	m_acCopy = new QAction(tr("Copy"), this);
	m_acCopy->setObjectName(QStringLiteral("acCopy"));
	m_acCopy->setShortcut(QKeySequence::Copy);
	m_acCopy->setShortcutContext(Qt::WidgetShortcut);
	m_acCopy->setEnabled(false);
	m_acCopy->setVisible(false);
	connect(m_acCopy, SIGNAL(triggered()), this, SLOT(copySelected()));
	addAction(m_acCopy);

	m_acPaste = new QAction(tr("Paste"), this);
	m_acPaste->setObjectName(QStringLiteral("acPaste"));
	m_acPaste->setShortcut(QKeySequence::Paste);
	m_acPaste->setShortcutContext(Qt::WidgetShortcut);
	m_acPaste->setEnabled(false);
	m_acPaste->setVisible(false);
	connect(m_acPaste, SIGNAL(triggered()), this, SLOT(pasteSelected()));
	addAction(m_acPaste);

	m_acCut = new QAction(tr("Cut"), this);
	m_acCut->setObjectName(QStringLiteral("acCut"));
	m_acCut->setShortcut(QKeySequence::Cut);
	m_acCut->setShortcutContext(Qt::WidgetShortcut);
	m_acCut->setEnabled(false);
	m_acCut->setVisible(false);
	connect(m_acCut, SIGNAL(triggered()), this, SLOT(cutSelected()));
	addAction(m_acCut);

	m_acDelete = new QAction(tr("Delete"), this);
	m_acDelete->setObjectName(QStringLiteral("acDelete"));
	m_acDelete->setShortcut(QKeySequence::Delete);
	m_acDelete->setShortcutContext(Qt::WidgetShortcut);
	m_acDelete->setEnabled(false);
	m_acDelete->setVisible(false);
	connect(m_acDelete, SIGNAL(triggered()), this, SLOT(deleteSelected()));
	addAction(m_acDelete);

	QClipboard *clpb = QApplication::clipboard();
	connect(clpb, SIGNAL(changed(QClipboard::Mode)), this, SLOT(onClipBoardChanged(QClipboard::Mode)));
}

void CSchemeEditor::setScheme(CScheme *a_scheme)
{
	if(scheme() && (scheme() == a_scheme)) return;
	if(scheme())
	{
		disconnect(scheme(), SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));
	}
	setScene(a_scheme);
	if(scheme())
	{
		connect(scheme(), SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));
	}
}

void CSchemeEditor::onSelectionChanged(void)
{
	bool hasSelected = scheme() && !scheme()->selectedItems().isEmpty();
	if(m_acCopy)
	{
		m_acCopy->setEnabled(hasSelected);
		m_acCopy->setVisible(hasSelected);
	}
	if(m_acCut)
	{
		m_acCut->setEnabled(hasSelected);
		m_acCut->setVisible(hasSelected);
	}
	if(m_acDelete)
	{
		m_acDelete->setEnabled(hasSelected);
		m_acDelete->setVisible(hasSelected);
	}
}

void CSchemeEditor::onClipBoardChanged(const QClipboard::Mode &mode)
{
	Q_UNUSED(mode)

	QClipboard *clpb = QApplication::clipboard();
	if(clpb->mimeData()->hasHtml())
	{
		QDomDocument domDoc;
		QString errMsg;
		int errLine = 0;
		int errCol = 0;
		if(domDoc.setContent(clpb->mimeData()->html(), &errMsg, &errLine, &errCol))
		{
			if(scheme() && scheme()->checkXMLSchemeFormat(domDoc))
			{
				if(m_acPaste)
				{
					m_acPaste->setEnabled(true);
					m_acPaste->setVisible(true);
				}
			}
		}
	}
}

void CSchemeEditor::copySelected(void)
{
	if(scheme())
	{
		QDomDocument domDoc = scheme()->toXMLDom(scheme()->selectedElements());
		QMimeData *mimeData = new QMimeData();
		mimeData->setHtml(domDoc.toString());
		QClipboard *clpb = QApplication::clipboard();
		clpb->setMimeData(mimeData);
	}
}

void CSchemeEditor::pasteSelected(void)
{
	QClipboard *clpb = QApplication::clipboard();
	if(clpb->mimeData()->hasHtml())
	{
		QDomDocument domDoc;
		QString errMsg;
		int errLine = 0;
		int errCol = 0;
		if(domDoc.setContent(clpb->mimeData()->html(), &errMsg, &errLine, &errCol))
		{
			if(scheme())
			{
				QList<CElement*> elements = scheme()->fromXMLDom(domDoc);
				scheme()->clearSelection();
				scheme()->addElements(elements);
				foreach(CElement *element, elements) element->setSelected(true);
			}
		}
	}
}

void CSchemeEditor::cutSelected(void)
{
}

void CSchemeEditor::deleteSelected(void)
{
	if(scheme()) scheme()->deleteSelected();
}