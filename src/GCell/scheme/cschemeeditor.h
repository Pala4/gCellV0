#ifndef CSCHEMEEDITOR_H
#define CSCHEMEEDITOR_H

#include <QGraphicsView>

#include <QMap>
#include <QGraphicsItem>
#include <QClipboard>

#include "algorithm/calgorithm.h"

class QActionGroup;

class CScheme;
class CElementOptionsWgt;
class CSegmentMover;

class CBounds
{
private:
	QRectF m_bounds;
public:
	CBounds(const QRectF &bounds = QRectF());

	const QRectF& bounds(void) const{return m_bounds;}
	void setBounds(const QRectF &bounds){m_bounds = bounds;}

	bool inBounds(const QPointF &pos) const{return m_bounds.contains(pos);}
	bool outBounds(const QPointF &pos, const QRectF &itemBoundingRect);
	QPointF posByBound(const QPointF &newPos, const QRectF &itemBoundingRect);
	void paint(QPainter *painter);
};

class CGrid
{
private:
	QColor m_color;
	QColor m_bkGndColor;
	int m_step;
	qreal m_pointSize;
	QBrush m_bkGndBrush;
	bool m_align;

	CBounds *m_bounds;

	void initBrush(void);
public:
	CGrid(CBounds *bounds = 0) : m_color(Qt::black), m_bkGndColor(Qt::white), m_step(8), m_pointSize(0.0), m_align(true), m_bounds(bounds){initBrush();}
	CGrid(const QColor &color, const QColor &bkGndColor, const int &step, const qreal &pointSize, const bool &align, CBounds *bounds = 0);

	const QColor& color(void) const{return m_color;}
	void setColor(const QColor &color){if(m_color == color) return; m_color = color; initBrush();}
	const QColor& bkGndColor(void) const{return m_bkGndColor;}
	void setBkGndColor(const QColor &bkGndColor){if(m_bkGndColor == bkGndColor) return; m_bkGndColor = bkGndColor; initBrush();}
	const int& step(void) const{return m_step;}
	void setStep(const int  &step){if(m_step == step) return; m_step = step; initBrush();}
	const qreal& pointSize(void) const{return m_pointSize;}
	void setPointSize(const qreal &pointSize){if(m_pointSize == pointSize) return; m_pointSize = pointSize; initBrush();}
	const bool& isAlign(void) const{return m_align;}
	void setAlign(const bool &align){m_align = align;}

	CBounds* bounds(void){return m_bounds;}
	void setBounds(CBounds *bounds){m_bounds = bounds;}

	QPointF align(const QPointF &pos);
	void paint(QPainter *painter);
};

template<typename Type>
class CItemMover
{
private:
	CGrid *m_greed;
	CBounds *m_bounds;
    QMap<QGraphicsItem*, QPointF> m_elementOffsets;
public:
	CItemMover(CGrid *greed = 0, CBounds *bounds = 0){m_greed = greed; m_bounds = bounds;}

	CGrid* greed(void) const{return m_greed;}
	void setGreed(CGrid *greed){m_greed = greed;}

	CBounds* bounds(void){return m_bounds;}
	void setBounds(CBounds *bounds){m_bounds = bounds;}

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
			if(m_bounds)
			{
				if(m_bounds->outBounds(newPos, item->boundingRect()))
				{
					newPos = item->pos();
				}
				else
				{
					newPos = m_bounds->posByBound(newPos, item->boundingRect());
				}
			}

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
	Q_PROPERTY(qreal gridPointSize READ gridPointSize WRITE setGridPointSize)
	Q_PROPERTY(bool gridAlign READ isGridAlign WRITE setGridAlign)
public:
	enum TMouseMode{MoveSelectMode, MoveSceneMode, LinkingMode, AddAlgorithmMode};
private:
//	QAction *m_acElementOptions;
//	QAction *m_acCopy;
//	QAction *m_acCut;
//	QAction *m_acDelete;
//	QAction *m_acPaste;

//	QActionGroup *m_generalElementActions;
//	QActionGroup *m_editActions;
//	QActionGroup *m_generalEditActions;

	QMenu *m_contextMenu;

	CSchemeEditor::TMouseMode m_mouseMode;

	CBounds m_bounds;
	CGrid m_grid;
	CItemMover<CAlgorithm*> m_algorithmMover;
	CSelector m_selector;

	CElementOptionsWgt *m_elementOptionsWgt;
	QSize m_mousePortalHookSize;
	CPortal *m_hockedPortal;
	CPortal *m_firstPortal;
	CPortal *m_secondPortal;
	CSegmentMover *m_segmentMover;

	qreal vecModul(const QPointF &point) const;
	CPortal* poratalUnderPortalHook(const QPoint &mousePos);

	CScheme* scheme(void);
protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void drawBackground(QPainter *painter, const QRectF &rect);
    virtual void drawForeground(QPainter *painter, const QRectF &rect);
public:
	explicit CSchemeEditor(QWidget *parent = 0);

	const QColor& gridColor(void) const{return m_grid.color();}
	void setGridColor(const QColor &gridColor);
	const QColor& gridBkGndColor(void) const{return m_grid.bkGndColor();}
	void setGridBkGndColor(const QColor &gridBkGndColor);
	const int& gridStep(void) const{return m_grid.step();}
	void setGridStep(const int &gridStep);
	const qreal& gridPointSize(void) const{return m_grid.pointSize();}
	void setGridPointSize(const qreal &gridPointSize);
	const bool& isGridAlign(void) const{return m_grid.isAlign();}
	void setGridAlign(const bool &gridAlign){m_grid.setAlign(gridAlign);}
	void setupGrid(const QColor &gridColor, const QColor &gridBkColor,
				   const int &gridStep, const qreal &gridPointSize,
				   const bool &gridAlign);

//	QList<QAction*> generalElementActions(void) const;
//	QList<QAction*> editActions(void) const;
//	QList<QAction*> generalEditActions(void) const;

	QMenu* contextMenu(void){return m_contextMenu;}
	void setContextMenu(QMenu *contextMenu){m_contextMenu = contextMenu;}

	void setScheme(CScheme *a_scheme);
	QList<CElement*> selectedElements(void);
private slots:
	void onSelectionChanged(void);
//	void onClipBoardChanged(const QClipboard::Mode &mode);
	void onSchemeRectChanged(const QRectF &rect);
	void updateTitle(void);
public slots:
	void setMouseMode(const CSchemeEditor::TMouseMode &mouseMode);

	void showElementOptions(void);
	void copySelected(void);
	void pasteSelected(void);
	void cutSelected(void);
	void deleteSelected(void);
signals:
	void addAlgorithmModeFinished(void);
	void windowTitleChanged(void);
	void elementsSelected(QList<CElement*> element);
};

#endif // CSCHEMEEDITOR_H