#ifndef CPORTAL_H
#define CPORTAL_H

#include <QGraphicsItem>

#include "../celement.h"
#include "../databuffer/cdatabuffer.h"

class QSignalMapper;

class CLink;

class CPortal : public CElement
{
	Q_OBJECT
	Q_PROPERTY(int portalOrientation READ portalOrientation WRITE setPortalOrientation)
	Q_PROPERTY(QColor dataColor READ dataColor WRITE setDataColor NOTIFY dataColorChanged)
public:
	enum TPortalOrientation{Left, Top, Right, Bottom};
private:
	CPortal::TPortalOrientation m_portalOrientation;
	QColor m_dataColor;

	QPainterPath m_portalForm;
	QRectF m_captionRect;
	QPointF m_linkPos;
	qreal m_size;

	bool m_hovered;
	bool m_checked;
	bool m_loopBackPortal;

	bool m_dataBufferIsReference;
	CDataBuffer *m_dataBuffer;
	QList<CLink*> m_links;

	QSignalMapper *m_orientActionsMapper;
	QAction *m_acLeftOrient;
	QAction *m_acTopOrient;
	QAction *m_acRightOrient;
	QAction *m_acBottomOrient;

	static QPainterPath shapeFromPath(const QPainterPath &path, const QPen &pen);
	virtual QPainterPath calcPortalForm(void){return QPainterPath();}
	void updateLinks(void);
protected:
	const qreal& size(void) const{return m_size;}
	void setLinkPos(const QPointF &linkPos){m_linkPos = linkPos;}
	virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
	QRectF calcBounds(void);
public:
	explicit CPortal(QGraphicsItem *parent = 0);

	virtual QPainterPath shape(void) const;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	const CPortal::TPortalOrientation& portalOrientation(void) const{return m_portalOrientation;}
	void setPortalOrientation(const CPortal::TPortalOrientation &portalOrientation);
	const QColor& dataColor(void) const{return m_dataColor;}
	void setDataColor(const QColor &dataColor);

	const bool& isChecked(void) const{return m_checked;}
	void setChecked(const bool &cheked);

	const QPointF& linkPos(void) const{return m_linkPos;}

	const bool& isLoopBackPortal(void) const{return m_loopBackPortal;}
	void setLoopBackPortal(const bool &loopBackPortal){if(m_loopBackPortal == loopBackPortal) return; m_loopBackPortal = loopBackPortal; update();}

	const QList<CLink*>& links(void) const{return m_links;}
	void addLink(CLink *link);
	void removeLink(CLink *link);
	bool isUsed(void) const{return !links().isEmpty();}

	virtual CDataBuffer* createBuffer(void);
	void setBuffer(CDataBuffer *dataBuffer);
	void clearBuffer(void);
	CDataBuffer* buffer(void){return m_dataBuffer;}

	void appendBuffer(const stTimeFrame &timeFrame, const qreal &value);
	stData bufferData(const int &index);
	bool isBufferDataExist(const stTimeFrame &frame);

	void operator << (const stData &data)
	{
		appendBuffer(data.timeFrame, data.value);
	}

	virtual void beforeCalc(const qreal &startTime, const qreal &timeStep, const qreal &endTime);
private slots:
	void setPortalOrientation(const int &portalOrientation);
public slots:
	virtual void updateGeometry(void);
signals:
	void dataColorChanged(QColor dataColor);
	void geometryChanged(void);
};

#endif // CPORTAL_H