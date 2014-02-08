#ifndef CSEGMENTMOVER_H
#define CSEGMENTMOVER_H

#include <QObject>

#include <QGraphicsObject>

class CLink;
class CLinkSegment;
class CSegmentMover;

class CLinkMoverPart : public QGraphicsObject
{
	Q_OBJECT
private:
	CSegmentMover *m_segmentMover;
	CLinkSegment *m_segment;

	bool m_moved;
	QPointF m_dP;
protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
public:
	explicit CLinkMoverPart(QGraphicsItem *parent = 0, CSegmentMover *segmentMover = 0);

	CSegmentMover* segmentMover(void){return m_segmentMover;}
	void setSegmentMover(CSegmentMover *segmentMover);

	CLinkSegment* segment(void){return m_segment;}
	void setSegment(CLinkSegment *segment);

	virtual QRectF boundingRect(void) const;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	void setup(void);
signals:
	void stateChanged(void);
};

class CSegmentMover : public QObject
{
	Q_OBJECT
	Q_PROPERTY(qreal partSize READ partSize WRITE setPartSize USER true)
	Q_PROPERTY(QColor borderNormalColor READ borderNormalColor WRITE setBorderNormalColor USER true)
	Q_PROPERTY(QColor fillNormalColor READ fillNormalColor WRITE setFillNormalColor USER true)
	Q_PROPERTY(QColor borderHoverColor READ borderHoverColor WRITE setBorderHoverColor USER true)
	Q_PROPERTY(QColor fillHoverColor READ fillHoverColor WRITE setFillHoverColor USER true)
	Q_PROPERTY(QColor borderMovedColor READ borderMovedColor WRITE setBorderMovedColor USER true)
	Q_PROPERTY(QColor fillMovedColor READ fillMovedColor WRITE setFillMovedColor USER true)
private:
	qreal m_partSize;
	QColor m_borderNormalColor;
	QColor m_fillNormalColor;
	QColor m_borderHoverColor;
	QColor m_fillHoverColor;
	QColor m_borderMovedColor;
	QColor m_fillMovedColor;

	CLink *m_link;
	QList<CLinkMoverPart*> m_parts;

	void setupParts(void);
public:
	explicit CSegmentMover(QObject *parent = 0);

	//Part size
	const qreal& partSize(void) const{return m_partSize;}
	void setPartSize(const qreal &partSize){m_partSize = partSize; setupParts();}
	//Normal color
	const QColor& borderNormalColor(void) const{return m_borderNormalColor;}
	void setBorderNormalColor(const QColor &borderNormalColor){m_borderNormalColor = borderNormalColor; setupParts();}
	const QColor& fillNormalColor(void) const{return m_fillNormalColor;}
	void setFillNormalColor(const QColor &fillNormalColor){m_fillNormalColor = fillNormalColor; setupParts();}
	//Hover color
	const QColor& borderHoverColor(void) const{return m_borderHoverColor;}
	void setBorderHoverColor(const QColor &borderHoverColor){m_borderHoverColor = borderHoverColor; setupParts();}
	const QColor& fillHoverColor(void) const{return m_fillHoverColor;}
	void setFillHoverColor(const QColor &fillHoverColor){m_fillHoverColor = fillHoverColor; setupParts();}
	//Moved color
	const QColor& borderMovedColor(void) const{return m_borderMovedColor;}
	void setBorderMovedColor(const QColor &borderMovedColor){m_borderMovedColor = borderMovedColor; setupParts();}
	const QColor& fillMovedColor(void) const{return m_fillMovedColor;}
	void setFillMovedColor(const QColor &fillMovedColor){m_fillMovedColor = fillMovedColor; setupParts();}

	CLink* link(void){return m_link;}
	void setLink(CLink *link);
private slots:
	void onPartStateChanged(void);
	void onLinkFormChanged(void);
};

#endif // CSEGMENTMOVER_H