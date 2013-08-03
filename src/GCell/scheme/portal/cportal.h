#ifndef CPORTAL_H
#define CPORTAL_H

#include <QGraphicsItem>

#include "../celement.h"

class QSignalMapper;

class CLink;
class CAbstractDataBuffer;

class CPortal : public CElement
{
	Q_OBJECT
	Q_PROPERTY(int portalOrientation READ portalOrientation WRITE setPortalOrientation)
public:
	enum TPortalOrientation{Left, Top, Right, Bottom};
private:
	CPortal::TPortalOrientation m_portalOrientation;
	QPainterPath m_portalForm;
	QPointF m_linkPos;
	qreal m_size;
	bool m_hovered;
	bool m_checked;

	bool m_loopBackPortal;

	bool m_dataBufferIsReference;
	CAbstractDataBuffer *m_dataBuffer;
	QList<CLink*> m_links;

	QSignalMapper *m_orientActionsMapper;
	QAction *m_acLeftOrient;
	QAction *m_acTopOrient;
	QAction *m_acRightOrient;
	QAction *m_acBottomOrient;

	static QPainterPath shapeFromPath(const QPainterPath &path, const QPen &pen);
	virtual QPainterPath calcPortalForm(void) = 0;
	void updateLinks(void);
protected:
	const qreal& size(void) const{return m_size;}
	void setLinkPos(const QPointF &linkPos){m_linkPos = linkPos;}
	virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
public:
	explicit CPortal(QGraphicsItem *parent = 0);

	virtual QPainterPath shape(void) const;
	virtual QRectF boundingRect(void) const;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	const CPortal::TPortalOrientation& portalOrientation(void) const{return m_portalOrientation;}
	void setPortalOrientation(const CPortal::TPortalOrientation &portalOrientation);

	const bool& isChecked(void) const{return m_checked;}
	void setChecked(const bool &cheked);

	const QPointF& linkPos(void) const{return m_linkPos;}

	const bool& isLoopBackPortal(void) const{return m_loopBackPortal;}
	void setLoopBackPortal(const bool &loopBackPortal){if(m_loopBackPortal == loopBackPortal) return; m_loopBackPortal = loopBackPortal; update();}

	const QList<CLink*>& links(void) const{return m_links;}
	void addLink(CLink *link);
	void removeLink(CLink *link);

	virtual CAbstractDataBuffer* createBuffer(void);
	void setBuffer(CAbstractDataBuffer *dataBuffer);
	void clearBuffer(void);
	CAbstractDataBuffer* buffer(void){return m_dataBuffer;}
	void addBufferData(const int &timeFrame, const qreal &data);
	qreal bufferData(const int &timeFrame);
	bool isBufferDataReady(const int &timeFrame);
private slots:
	void setPortalOrientation(const int &portalOrientation);
public slots:
	virtual void updateGeometry(void);
signals:
	void geometryChanged(void);
};

#endif // CPORTAL_H