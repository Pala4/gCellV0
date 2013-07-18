#ifndef CSCHEMEVIEW_H
#define CSCHEMEVIEW_H

#include <QGraphicsView>

#include <QMap>
#include <QGraphicsItem>

template <typename Type>
class CItemMover
{
private:
    QMap<QGraphicsItem*, QPointF> m_elementOffsets;
public:
    CItemMover(void){}

    bool haveMoved(void){return !m_elementOffsets.isEmpty();}

    void addElement(QGraphicsItem *item, const QPointF &mousePos)
    {
        if(!dynamic_cast<Type>(item)) return;
        if(m_elementOffsets.contains(item)) return;

        m_elementOffsets[item] = mousePos - item->pos();
    }
    void addElements(QList<QGraphicsItem*> items, const QPointF &mousePos)
    {
        foreach(QGraphicsItem *item, items)
        {
            addElement(item, mousePos);
        }
    }
    void move(const QPointF &mousePos)
    {
        for(int ci = 0; ci < m_elementOffsets.count(); ++ci)
        {
            QGraphicsItem *item = m_elementOffsets.keys().at(ci);
            if(!item) continue;
            QPointF dp = m_elementOffsets.values().at(ci);

            item->setPos(mousePos - dp);
        }
    }
    void release(const QPointF &mousePos = QPointF())
    {
        Q_UNUSED(mousePos)
        m_elementOffsets.clear();
    }
};

class CSchemeView : public QGraphicsView
{
    Q_OBJECT
public:
    enum TMouseMode{MoveSelectMode, MoveSceneMode, AddElementMode};
private:
    CSchemeView::TMouseMode m_mouseMode;
protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
public:
    explicit CSchemeView(QWidget *parent = 0);
public slots:
    void setMouseMode(const CSchemeView::TMouseMode &mouseMode){m_mouseMode = mouseMode;}
signals:
    void queryAddElement(QPointF pos);
    void mouseReleased(QPointF pos);
};

#endif // CSCHEMEVIEW_H