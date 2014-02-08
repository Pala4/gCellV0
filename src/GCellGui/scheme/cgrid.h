#ifndef CGRID_H
#define CGRID_H

#include <QColor>
#include <QBrush>

class QPainter;

class CBounds;

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

#endif // CGRID_H