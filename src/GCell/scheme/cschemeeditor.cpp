#include "cschemeeditor.h"

#include <QActionGroup>
#include <QMenu>
#include <QContextMenuEvent>
#include <QApplication>
#include <QMimeData>
#include <QLineEdit>

#include <qmath.h>

#include "cscheme.h"
#include "celementoptionswgt.h"
#include "portal/cportal.h"
#include "link/clink.h"
#include "link/csegmentmover.h"

using namespace gcell;
using namespace scheme;

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
qreal CSchemeEditor::vecModul(const QPointF &point) const
{
	if(point.isNull()) return 0.0;

	return qSqrt(qPow(point.x(), 2.0) + qPow(point.y(), 2.0));
}

CPortal* CSchemeEditor::poratalUnderPortalHook(const QPoint &mousePos)
{
	QRect hookRect = QRect(QPoint(0, 0), m_mousePortalHookSize);
	hookRect.moveCenter(mousePos);
	QList<QGraphicsItem*> hookedItems = items(hookRect, Qt::IntersectsItemBoundingRect);

	QList<CPortal*> hookedPortals;
	foreach(QGraphicsItem *hookedItem, hookedItems)
	{
		CPortal *hookedPortal = dynamic_cast<CPortal*>(hookedItem);
		if(hookedPortal) hookedPortals << hookedPortal;
	}
	if(hookedPortals.isEmpty()) return 0;
	if(hookedPortals.count() == 1) return hookedPortals.at(0);

	qreal minDistance = vecModul(hookedPortals.at(0)->mapToScene(hookedPortals.at(0)->linkPos()) - mapToScene(mousePos));
	CPortal *hookedPortal = hookedPortals.at(0);
	for(int ci = 1; ci < hookedPortals.count(); ++ci)
	{
		qreal minDistanceTmp = vecModul(hookedPortals.at(ci)->mapToScene(hookedPortals.at(ci)->linkPos()) - mapToScene(mousePos));
		if(minDistanceTmp < minDistance)
		{
			minDistance = minDistanceTmp;
			hookedPortal = hookedPortals.at(ci);
		}
	}

	return hookedPortal;
}

CScheme* CSchemeEditor::scheme(void)
{
	return qobject_cast<CScheme*>(scene());
}

void CSchemeEditor::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu *menu = 0;
	bool delMenu = false;
	if(m_contextMenu && !m_contextMenu->isEmpty())
	{
		menu = m_contextMenu;
	}
	else
	{
		delMenu = true;
		menu = new QMenu(this);
		CElement *element = 0;
		QGraphicsItem *item = itemAt(event->pos());
		if(item && (!item->isSelected() || (item->isSelected() && scene() && (scene()->selectedItems().count() == 1))))
		{
			element = dynamic_cast<CElement*>(item);
		}
		if(element)
		{
			if(!element->actions().isEmpty())
			{
				menu->addActions(element->actions());
				menu->addSeparator();
			}
		}
		menu->addActions(actions());
	}

	if(menu)
	{
		bool showMenu = false;
		foreach(QAction *ac, menu->actions())
		{
			if(ac->isVisible()){showMenu = true; break;}
		}
		if(showMenu) menu->exec(event->globalPos());

		if(delMenu) menu->deleteLater();
	}
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
				if(m_hockedPortal)
				{
					if(!m_firstPortal)
					{
						m_firstPortal = m_hockedPortal;
					}
					else if(!m_secondPortal)
					{
						m_secondPortal = m_hockedPortal;
					}
					m_hockedPortal->setHighLighted(false);
					m_hockedPortal->setChecked(true);
					m_hockedPortal = 0;
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
				if(scheme())
				{
					QPointF pos = mapToScene(event->pos());
					if(m_bounds.inBounds(pos))
					{
						scheme()->createAlgorithm(m_grid.align(pos));
						emit addAlgorithmModeFinished();
					}
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
			break;
		}
		case CSchemeEditor::LinkingMode:
		{
			CPortal *hockedPortal = poratalUnderPortalHook(event->pos());
			if(hockedPortal && hockedPortal->canLinked())
			{
				if(m_hockedPortal && (m_hockedPortal != hockedPortal)) m_hockedPortal->setHighLighted(false);
				m_hockedPortal = hockedPortal;
				m_hockedPortal->setHighLighted(true);
			}
			else
			{
				if(m_hockedPortal) m_hockedPortal->setHighLighted(false);
				m_hockedPortal = 0;
			}
			break;
		}
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
}

void CSchemeEditor::drawBackground(QPainter *painter, const QRectF &rect)
{
	Q_UNUSED(rect)
	m_bounds.paint(painter);
	m_grid.paint(painter);
}

void CSchemeEditor::drawForeground(QPainter *painter, const QRectF &rect)
{
	Q_UNUSED(rect)
	if(m_selector.isStarted())
	{
		m_selector.draw(painter);
	}
}

CSchemeEditor::CSchemeEditor(QWidget *parent) : QGraphicsView(parent), mainwindow::workspace::IView()
{
	setObjectName(QStringLiteral("CSchemeView"));

	m_contextMenu = 0;

	m_mouseMode = CSchemeEditor::MoveSelectMode;
	m_elementOptionsWgt = 0;
	m_mousePortalHookSize = QSize(20, 20);
	m_hockedPortal = 0;
	m_firstPortal = 0;
	m_secondPortal = 0;
	m_segmentMover = 0;

	m_grid.setBounds(&m_bounds);

	m_segmentMover = new CSegmentMover(this);
	m_segmentMover->setObjectName(QStringLiteral("segmentMover"));

	setAlignment(Qt::AlignLeft | Qt::AlignTop);
	setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
	setMouseTracking(true);
}

void CSchemeEditor::setGridColor(const QColor &gridColor)
{
	if(m_grid.color() == gridColor) return;

	m_grid.setColor(gridColor);
	update();
}

void CSchemeEditor::setGridBkGndColor(const QColor &gridBkGndColor)
{
	if(m_grid.bkGndColor() == gridBkGndColor) return;

	m_grid.setBkGndColor(gridBkGndColor);
	update();
}

void CSchemeEditor::setGridStep(const int &gridStep)
{
	if(m_grid.step() == gridStep) return;

	m_grid.setStep(gridStep);
	update();
}

void CSchemeEditor::setGridPointSize(const qreal &gridPointSize)
{
	if(m_grid.pointSize() == gridPointSize) return;

	m_grid.setPointSize(gridPointSize);
	update();
}

void CSchemeEditor::setupGrid(const QColor &gridColor, const QColor &gridBkColor,
							  const int &gridStep, const qreal &gridPointSize,
							  const bool &gridAlign)
{
	if((m_grid.color() == gridColor) &&
	   (m_grid.bkGndColor() == gridBkColor) &&
	   (m_grid.step() == gridStep) &&
	   (m_grid.pointSize() == gridPointSize) &&
	   (m_grid.isAlign() == gridAlign)) return;

	m_grid.setColor(gridColor);
	m_grid.setBkGndColor(gridBkColor);
	m_grid.setStep(gridStep);
	m_grid.setPointSize(gridPointSize);
	m_grid.setAlign(gridAlign);

	update();
}

void CSchemeEditor::setScheme(CScheme *a_scheme)
{
	if(scheme() && (scheme() == a_scheme)) return;
	if(scheme())
	{
		disconnect(scheme(), SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));
		disconnect(scheme(), SIGNAL(fileNameChanged(QString)), this, SLOT(updateTitle()));
		disconnect(scheme(), SIGNAL(sceneRectChanged(QRectF)), this, SLOT(onSchemeRectChanged(QRectF)));
		disconnect(scheme(), SIGNAL(elementAdded(CElement*)), this, SLOT(onSchemeElementAdedd(CElement*)));
		disconnect(scheme(), SIGNAL(schemeModified(bool)), this, SLOT(onSchemeModified(bool)));

		foreach (CElement *element, scheme()->elements())
		{
			if(!element) continue;
			element->setGrid(0);
		}
	}
	setScene(a_scheme);
	if(scheme())
	{
		connect(scheme(), SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));
		connect(scheme(), SIGNAL(fileNameChanged(QString)), this, SLOT(updateTitle()));
		connect(scheme(), SIGNAL(sceneRectChanged(QRectF)), this, SLOT(onSchemeRectChanged(QRectF)));
		connect(scheme(), SIGNAL(elementAdded(CElement*)), this, SLOT(onSchemeElementAdedd(CElement*)));
		connect(scheme(), SIGNAL(schemeModified(bool)), this, SLOT(onSchemeModified(bool)));

		m_bounds.setBounds(scheme()->sceneRect());
		foreach (CElement *element, scheme()->elements())
		{
			if(!element) continue;
			element->setGrid(&m_grid);
		}
	}

	centerOn(0.0, 0.0);
	updateTitle();
	update();
}

QList<CElement*> CSchemeEditor::selectedElements(void)
{
	return scheme() ? scheme()->selectedElements() : QList<CElement*>();
}

void CSchemeEditor::onSchemeElementAdedd(CElement *element)
{
	if(!element) return;
	element->setGrid(&m_grid);
}

void CSchemeEditor::onSelectionChanged(void)
{
	if(scheme())
	{
		CLink *link = 0;
		if(scheme()->selectedElements().count() == 1)
		{
			link = dynamic_cast<CLink*>(scheme()->selectedItems().at(0));
		}
		if(m_segmentMover) m_segmentMover->setLink(link);
		emit elementsSelected(scheme()->selectedElements());
	}
}

void CSchemeEditor::onSchemeRectChanged(const QRectF &rect)
{
	m_bounds.setBounds(rect);
	update();
}

void CSchemeEditor::onSchemeModified(const bool &modified)
{
	setWindowModified(modified);
	updateTitle();
}

void CSchemeEditor::updateTitle(void)
{
	QString title = scheme() ? scheme()->fileName() : QString();
	title += isWindowModified() ? " *" : QString();
	setWindowTitle(title);

	emit windowTitleChanged();
}

void CSchemeEditor::setMouseMode(const CSchemeEditor::TMouseMode &mouseMode)
{
	if(m_mouseMode == mouseMode) return;
	m_mouseMode = mouseMode;
}

void CSchemeEditor::showElementOptions(void)
{
	if((scheme() && scheme()->selectedItems().count() == 1))
	{
		CElement *element = dynamic_cast<CElement*>(scheme()->selectedItems().at(0));
		if(element)
		{
			CElementOptionsWgt *optWgt = element->optionsWidget(this);
			if(optWgt)
			{
				int result = optWgt->exec();
				if(result == QDialog::Accepted)
				{
					element->acceptOptions(optWgt);
				}
				optWgt->deleteLater();
			}
		}
	}
}

void CSchemeEditor::copySelected(void)
{
	if(scheme())
	{
		QList<CElement*> elementsToCopy;
		foreach(CElement *element, scheme()->selectedElements())
		{
			if(element && (element->intercations() & CElement::Copyable)) elementsToCopy << element;
		}
		QDomDocument domDoc = scheme()->toXMLDom(elementsToCopy);
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
	copySelected();
	deleteSelected();
}

void CSchemeEditor::deleteSelected(void)
{
	if(scheme()) scheme()->deleteSelected();
}