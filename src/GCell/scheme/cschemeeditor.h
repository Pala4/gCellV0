#ifndef CSCHEMEEDITOR_H
#define CSCHEMEEDITOR_H

#include <QGraphicsView>

#include <QMap>
#include <QGraphicsItem>
#include <QClipboard>

#include "iview.h"

#include "cbounds.h"
#include "cgrid.h"
#include "algorithm/calgorithm.h"

class QActionGroup;

class CScheme;
class CElementOptionsWgt;
class CSegmentMover;

template<typename Type>
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

	QMenu* contextMenu(void){return m_contextMenu;}
	void setContextMenu(QMenu *contextMenu){m_contextMenu = contextMenu;}

	void setScheme(CScheme *a_scheme);
	QList<CElement*> selectedElements(void);
private slots:
	void onSchemeElementAdedd(CElement *element);
	void onSelectionChanged(void);
	void onSchemeRectChanged(const QRectF &rect);
	void onSchemeModified(const bool &modified);
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
