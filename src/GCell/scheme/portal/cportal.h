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
	enum TPortalOrientation{NoOrientation, Left, Top, Right, Bottom};
private:
	QString m_captionPrefix;
	QString m_captionPostfix;
	CPortal::TPortalOrientation m_portalOrientation;
	QColor m_dataColor;

	QPainterPath m_portalForm;
	QPointF m_linkPos;
	qreal m_size;

	bool m_highLighted;
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
	virtual QRectF calcBounds(void);
	virtual QPointF captionEditorPosition(void);
public:
	explicit CPortal(QGraphicsItem *parent = 0);

	virtual QString caption(void) const;
	const QString& captionPrefix(void) const{return m_captionPrefix;}
	void setCaptionPrefix(const QString &captionPrefix);
	const QString& captionPostfix(void) const{return m_captionPostfix;}
	void setCaptionPostfix(const QString &captionPostfix);

	virtual QPainterPath shape(void) const;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	const CPortal::TPortalOrientation& portalOrientation(void) const{return m_portalOrientation;}
	void setPortalOrientation(const CPortal::TPortalOrientation &portalOrientation);
	const QColor& dataColor(void) const{return m_dataColor;}
	void setDataColor(const QColor &dataColor);

	const bool& isHighLighted(void) const{return m_highLighted;}
	void setHighLighted(const bool &highLighted);
	const bool& isChecked(void) const{return m_checked;}
	void setChecked(const bool &cheked);

	const QPointF& linkPos(void) const{return m_linkPos;}

	const bool& isLoopBackPortal(void) const{return m_loopBackPortal;}
	void setLoopBackPortal(const bool &loopBackPortal){if(m_loopBackPortal == loopBackPortal) return; m_loopBackPortal = loopBackPortal; update();}

	const QList<CLink*>& links(void) const{return m_links;}
	void addLink(CLink *link);
	void removeLink(CLink *link);
	bool isUsed(void) const{return !links().isEmpty();}
	virtual bool canLinked(void){return true;}

	CElement* hostElement(void){return dynamic_cast<CElement*>(parentItem());}

	virtual CDataBuffer* createBuffer(void);
	void setBuffer(CDataBuffer *dataBuffer);
	void clearBuffer(void);
	CDataBuffer* buffer(void){return m_dataBuffer;}

	quint64 bufferSize(void) const;
	void appendBuffer(const stTimeFrame &timeFrame, const qreal &value);
	virtual stData bufferData(const quint64 &timeFrameIndex);
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