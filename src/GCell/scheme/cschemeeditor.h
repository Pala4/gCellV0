#ifndef CSCHEMEEDITOR_H
#define CSCHEMEEDITOR_H

#include <QGraphicsView>

#include <QMap>
#include <QGraphicsItem>
#include <QClipboard>

#include "algorithm/calgorithm.h"

class CScheme;
class CElementOptionsWgt;

class CGreed
{
private:
	QColor m_color;
	QColor m_bkGndColor;
	int m_step;
public:
	CGreed(void) : m_color(Qt::black), m_bkGndColor(Qt::white), m_step(8){}
	CGreed(const QColor &color, const QColor &bkGndColor, const int &step);

	QBrush bkGndBrush(void);
	QPointF align(const QPointF &pos);
};

template<typename Type>
class CItemMover
{
private:
	CGreed *m_greed;
    QMap<QGraphicsItem*, QPointF> m_elementOffsets;
public:
	CItemMover(CGreed *greed = 0){m_greed = greed;}

	CGreed* greed(void) const{return m_greed;}
	void setGreed(CGreed *greed){m_greed = greed;}

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

			QPointF newPos = mousePos - dp;
			if(m_greed) newPos = m_greed->align(newPos);

			item->setPos(newPos);
        }
    }
	void release(const QPointF &mousePos)
    {
		move(mousePos);
		release();
    }
	void release(void)
	{
		m_elementOffsets.clear();
	}
};

class CSelector
{
private:
    QRectF m_selectorRect;
    bool m_started;
public:
    CSelector(void);

    const QRectF& selectorRect(void) const{return m_selectorRect;}
    const bool& isStarted(void) const{return m_started;}
    void start(const QPointF &pos);
    void move(const QPointF &pos);
    QRectF release(const QPointF &pos);

    void draw(QPainter *painter);
};

class CSchemeEditor : public QGraphicsView
{
    Q_OBJECT
public:
	enum TMouseMode{MoveSelectMode, MoveSceneMode, LinkingMode, AddAlgorithmMode};
private:
	QAction *m_acElementOptions;
	QAction *m_acElementActionsSeparator;
	QAction *m_acCopy;
	QAction *m_acPaste;
	QAction *m_acCut;
	QAction *m_acDelete;
	CSchemeEditor::TMouseMode m_mouseMode;
	CGreed m_greed;
	CItemMover<CAlgorithm*> m_algorithmMover;
    CSelector m_selector;
	CElementOptionsWgt *m_elementOptionsWgt;
	QSize m_mousePortalHookSize;
	CPortal *m_hockedPortal;
	CPortal *m_firstPortal;
	CPortal *m_secondPortal;

	qreal vecModul(const QPointF &point) const;
	CPortal* poratalUnderPortalHook(const QPoint &mousePos);

	CScheme* scheme(void);
protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void drawForeground(QPainter *painter, const QRectF &rect);
public:
	explicit CSchemeEditor(QWidget *parent = 0);

	void setScheme(CScheme *a_scheme);

	QList<QAction*> schemeEditorActions(void);
private slots:
	void onSelectionChanged(void);
	void onClipBoardChanged(const QClipboard::Mode &mode);
public slots:
	void setMouseMode(const CSchemeEditor::TMouseMode &mouseMode);

	void showOptions(void);
	void copySelected(void);
	void pasteSelected(void);
	void cutSelected(void);
	void deleteSelected(void);
signals:
    void mouseReleased(QPointF pos);
};

#endif // CSCHEMEEDITOR_H