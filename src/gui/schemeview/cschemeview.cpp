#include "cschemeview.h"

#include <QMenu>
#include <QContextMenuEvent>

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
void CSchemeView::contextMenuEvent(QContextMenuEvent *event)
{
    QGraphicsView::contextMenuEvent(event);

    if(!event->isAccepted())
    {
        QMenu menu;
        menu.addActions(actions());
        menu.exec(event->globalPos());
    }
}

void CSchemeView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);

    switch((int)m_mouseMode)
    {
    case CSchemeView::MoveSelectMode:
    {
        if(event->buttons() & Qt::LeftButton)
        {
            if(itemAt(event->pos()))
            {
                m_elementMover.release();
                if(scene()) m_elementMover.addElements(scene()->selectedItems(), mapToScene(event->pos()));
            }
            else
            {
                m_selector.start(mapToScene(event->pos()));
                if(scene()) scene()->update();
            }
        }
        break;
    }
    case CSchemeView::MoveSceneMode:break;
    case CSchemeView::AddElementMode:
    {
        if(event->buttons() & Qt::LeftButton)
        {
            emit queryAddElement(mapToScene(event->pos()));
        }
        break;
    }
    }
}

void CSchemeView::mouseMoveEvent(QMouseEvent *event)
{
    switch((int)m_mouseMode)
    {
    case CSchemeView::MoveSelectMode:
    {
        if(event->buttons() & Qt::LeftButton)
        {
            if(m_elementMover.haveMoved())
            {
                m_elementMover.move(mapToScene(event->pos()));
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
    }
}

void CSchemeView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);

    m_elementMover.release(mapToScene(event->pos()));
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
    emit mouseReleased(mapToScene(event->pos()));
}

void CSchemeView::drawForeground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect)
    if(m_selector.isStarted())
    {
        m_selector.draw(painter);
    }
}

CSchemeView::CSchemeView(QWidget *parent) : QGraphicsView(parent)
{
    setObjectName(QStringLiteral("CSchemeView"));

    m_mouseMode = CSchemeView::MoveSelectMode;
}