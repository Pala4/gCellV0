#include "cschemeview.h"

#include <QMenu>
#include <QContextMenuEvent>

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
    switch((int)m_mouseMode)
    {
    case CSchemeView::MoveSelectMode:break;
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

void CSchemeView::mouseReleaseEvent(QMouseEvent *event)
{
    emit mouseReleased(mapToScene(event->pos()));
}

CSchemeView::CSchemeView(QWidget *parent) : QGraphicsView(parent)
{
    setObjectName(QStringLiteral("CSchemeView"));

    m_mouseMode = CSchemeView::MoveSelectMode;
}