#include "cschemeeditor.h"

#include <QMenu>
#include <QContextMenuEvent>
#include <QApplication>
#include <QMimeData>
#include <QLineEdit>

#include <qmath.h>

#include "cscheme.h"
#include "celementoptionswgt.h"
#include "portal/cportal.h"

/*!
 * \class CGreed
 */
CGrid::CGrid(const QColor &color, const QColor &bkGndColor, const int &step, const bool &align)
{
	m_color = color;
	m_bkGndColor = bkGndColor;
	m_step = step;
	m_align = align;
}

QBrush CGrid::bkGndBrush(void)
{
	QPen pen;
	pen.setWidth(0);
	pen.setColor(m_color);

	QPixmap pixmap(m_step, m_step);
	pixmap.fill(m_bkGndColor);

	QPainter bkGndPainter(&pixmap);
	bkGndPainter.setPen(pen);
	bkGndPainter.drawPoint(0.0, 0.0);

	return QBrush(pixmap);
}

QPointF CGrid::align(const QPointF &pos)
{
	if(m_step == 0 || !isAlign()) return pos;

	qreal numStepsX = pos.x()/qreal(m_step);
	qreal modNumStepsX = qFabs(numStepsX - (int)numStepsX);
	qreal alX = (modNumStepsX > 0.5) ? (qreal)qCeil(numStepsX)*m_step : (qreal)qFloor(numStepsX)*m_step;

	qreal numStepsY = pos.y()/qreal(m_step);
	qreal modNumStepsY = qFabs(numStepsY - (int)numStepsY);
	qreal alY = (modNumStepsY > 0.5) ? (qreal)qCeil(numStepsY)*m_step : (qreal)qFloor(numStepsY)*m_step;

	return QPointF(alX, alY);
}

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
	QMenu menu;

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
			menu.addActions(element->actions());
			menu.addSeparator();
		}
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
				if(scheme()) scheme()->createAlgorithm(m_grid.align(mapToScene(event->pos())));
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

	m_acElementOptions = 0;
	m_acCopy = 0;
	m_acPaste = 0;
	m_acCut = 0;
	m_acDelete = 0;
	m_mouseMode = CSchemeEditor::MoveSelectMode;
	m_elementOptionsWgt = 0;
	m_mousePortalHookSize = QSize(20, 20);
	m_hockedPortal = 0;
	m_firstPortal = 0;
	m_secondPortal = 0;

	m_acElementOptions = new QAction(tr("Element options..."), this);
	m_acElementOptions->setObjectName(QStringLiteral("acElementOptions"));
	m_acElementOptions->setEnabled(false);
	m_acElementOptions->setVisible(false);
	connect(m_acElementOptions, SIGNAL(triggered()), this, SLOT(showOptions()));
	addAction(m_acElementOptions);

	m_acElementActionsSeparator = new QAction(this);
	m_acElementActionsSeparator->setObjectName("acElementActionsSeparator");
	m_acElementActionsSeparator->setSeparator(true);
	m_acElementActionsSeparator->setEnabled(false);
	m_acElementActionsSeparator->setVisible(false);
	addAction(m_acElementActionsSeparator);

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

	m_algorithmMover.setGreed(&m_grid);

	QClipboard *clpb = QApplication::clipboard();
	connect(clpb, SIGNAL(changed(QClipboard::Mode)), this, SLOT(onClipBoardChanged(QClipboard::Mode)));

	setBackgroundBrush(m_grid.bkGndBrush());
}

void CSchemeEditor::setGridColor(const QColor &gridColor)
{
	if(m_grid.color() == gridColor) return;

	m_grid.setColor(gridColor);
	setBackgroundBrush(m_grid.bkGndBrush());
}

void CSchemeEditor::setGridBkGndColor(const QColor &gridBkGndColor)
{
	if(m_grid.bkGndColor() == gridBkGndColor) return;

	m_grid.setBkGndColor(gridBkGndColor);
	setBackgroundBrush(m_grid.bkGndBrush());
}

void CSchemeEditor::setGridStep(const int &gridStep)
{
	if(m_grid.step() == gridStep) return;

	m_grid.setStep(gridStep);
	setBackgroundBrush(m_grid.bkGndBrush());
}

void CSchemeEditor::setupGrid(const QColor &gridColor, const QColor &gridBkColor,
							  const int &gridStep, const bool &gridAlign)
{
	if((m_grid.color() == gridColor) &&
	   (m_grid.bkGndColor() == gridBkColor) &&
	   (m_grid.step() == gridStep) &&
	   (m_grid.isAlign() == gridAlign)) return;

	m_grid.setColor(gridColor);
	m_grid.setBkGndColor(gridBkColor);
	m_grid.setStep(gridStep);
	m_grid.setAlign(gridAlign);

	setBackgroundBrush(m_grid.bkGndBrush());
}

void CSchemeEditor::setScheme(CScheme *a_scheme)
{
	if(scheme() && (scheme() == a_scheme)) return;
	if(scheme())
	{
		disconnect(scheme(), SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));
		disconnect(scheme(), SIGNAL(fileNameChanged(QString)), this, SLOT(updateTitle()));
		disconnect(scheme(), SIGNAL(sceneRectChanged(QRectF)), this, SLOT(onSchemeRectChanged(QRectF)));
	}
	setScene(a_scheme);
	if(scheme())
	{
		connect(scheme(), SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));
		connect(scheme(), SIGNAL(fileNameChanged(QString)), this, SLOT(updateTitle()));
		connect(scheme(), SIGNAL(sceneRectChanged(QRectF)), this, SLOT(onSchemeRectChanged(QRectF)));
	}
	centerOn(0.0, 0.0);

	updateTitle();
}

void CSchemeEditor::onSelectionChanged(void)
{
	bool hasSelected = (scheme() && !scheme()->selectedItems().isEmpty());
	bool ownSelect = (scheme() && scheme()->selectedItems().count() == 1);
	if(m_acElementOptions)
	{
		m_acElementOptions->setEnabled(ownSelect);
		m_acElementOptions->setVisible(ownSelect);
		if(m_acElementActionsSeparator) m_acElementActionsSeparator->setVisible(ownSelect);
	}
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

void CSchemeEditor::onSchemeRectChanged(const QRectF &rect)
{
	Q_UNUSED(rect)
	centerOn(0.0, 0.0);
}

void CSchemeEditor::updateTitle(void)
{
	QString title = scheme() ? scheme()->fileName() : QString();
	setWindowTitle(title);

	emit windowTitleChanged();
}

void CSchemeEditor::setMouseMode(const CSchemeEditor::TMouseMode &mouseMode)
{
	if(m_mouseMode == mouseMode) return;
	m_mouseMode = mouseMode;

	setMouseTracking((m_mouseMode == CSchemeEditor::LinkingMode));
}

void CSchemeEditor::showOptions(void)
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
    copySelected();
    deleteSelected();
}

void CSchemeEditor::deleteSelected(void)
{
	if(scheme()) scheme()->deleteSelected();
}