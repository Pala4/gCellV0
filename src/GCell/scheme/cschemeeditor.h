#ifndef CSCHEMEEDITOR_H
#define CSCHEMEEDITOR_H

#include <QGraphicsView>

#include <QMap>
#include <QGraphicsItem>
#include <QClipboard>

#include "algorithm/calgorithm.h"

class CScheme;
class CElementOptionsWgt;

class CGrid
{
private:
	QColor m_color;
	QColor m_bkGndColor;
	int m_step;
	bool m_align;
public:
	CGrid(void) : m_color(Qt::black), m_bkGndColor(Qt::white), m_step(8), m_align(true){}
	CGrid(const QColor &color, const QColor &bkGndColor, const int &step, const bool &align);

	const QColor& color(void) const{return m_color;}
	void setColor(const QColor &color){m_color = color;}
	const QColor& bkGndColor(void) const{return m_bkGndColor;}
	void setBkGndColor(const QColor &bkGndColor){m_bkGndColor = bkGndColor;}
	const int& step(void) const{return m_step;}
	void setStep(const int  &step){m_step = step;}
	const bool& isAlign(void) const{return m_align;}
	void setAlign(const bool &align){m_align = align;}

	QBrush bkGndBrush(void);
	QPointF align(const QPointF &pos);
};

template<typename Type>
class CItemMover
{
private:
	CGrid *m_greed;
    QMap<QGraphicsItem*, QPointF> m_elementOffsets;
public:
	CItemMover(CGrid *greed = 0){m_greed = greed;}

	CGrid* greed(void) const{return m_greed;}
	void setGreed(CGrid *greed){m_greed = greed;}

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
	Q_PROPERTY(QColor gridColor READ gridColor WRITE setGridColor)
	Q_PROPERTY(QColor gridBkGndColor READ gridBkGndColor WRITE setGridBkGndColor)
	Q_PROPERTY(int gridStep READ gridStep WRITE setGridStep)
	Q_PROPERTY(bool gridAlign READ isGridAlign WRITE setGridAlign)
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
	CGrid m_grid;
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

	const QColor& gridColor(void) const{return m_grid.color();}
	const QColor& gridBkGndColor(void) const{return m_grid.bkGndColor();}
	const int& gridStep(void) const{return m_grid.step();}
	const bool& isGridAlign(void) const{return m_grid.isAlign();}
	void setGridColor(const QColor &gridColor);
	void setGridBkGndColor(const QColor &gridBkGndColor);
	void setGridStep(const int &gridStep);
	void setGridAlign(const bool &gridAlign){m_grid.setAlign(gridAlign);}
	void setupGrid(const QColor &gridColor, const QColor &gridBkColor,
				   const int &gridStep, const bool &gridAlign);

	void setScheme(CScheme *a_scheme);
	QList<QAction*> schemeEditorActions(void);
private slots:
	void onSelectionChanged(void);
	void onClipBoardChanged(const QClipboard::Mode &mode);
	void onSchemeRectChanged(const QRectF &rect);
	void updateTitle(void);
public slots:
	void setMouseMode(const CSchemeEditor::TMouseMode &mouseMode);

	void showOptions(void);
	void copySelected(void);
	void pasteSelected(void);
	void cutSelected(void);
	void deleteSelected(void);
signals:
    void mouseReleased(QPointF pos);
	void windowTitleChanged(void);
};

#endif // CSCHEMEEDITOR_H