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
    void updateLinks();
protected:
    const qreal& size() const{return m_size;}
	void setLinkPos(const QPointF &linkPos){m_linkPos = linkPos;}
	virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);
    virtual QRectF calcBounds();
    virtual QPointF captionEditorPosition();
public:
	explicit CPortal(QGraphicsItem *parent = 0);

    virtual QString caption();
    const QString& captionPrefix() const{return m_captionPrefix;}
	void setCaptionPrefix(const QString &captionPrefix);
    const QString& captionPostfix() const{return m_captionPostfix;}
	void setCaptionPostfix(const QString &captionPostfix);

    QPainterPath shape() const;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    const CPortal::TPortalOrientation& portalOrientation() const{return m_portalOrientation;}
    void setPortalOrientation(const TPortalOrientation &portalOrientation);
    const QColor& dataColor() const{return m_dataColor;}
	void setDataColor(const QColor &dataColor);

    const bool& isHighLighted() const{return m_highLighted;}
	void setHighLighted(const bool &highLighted);
    const bool& isChecked() const{return m_checked;}
	void setChecked(const bool &cheked);

    const QPointF& linkPos() const{return m_linkPos;}

    const bool& isLoopBackPortal() const{return m_loopBackPortal;}
    void setLoopBackPortal(const bool &loopBackPortal);

    const QList<CLink*>& links() const{return m_links;}
	void addLink(CLink *link);
	void removeLink(CLink *link);
    bool isUsed() const{return !links().isEmpty();}
    virtual bool canLinked(){return true;}

    CElement* hostElement(){return dynamic_cast<CElement*>(parentItem());}

    virtual CDataBuffer* createBuffer();
	void setBuffer(CDataBuffer *dataBuffer);
    void clearBuffer();
    CDataBuffer* buffer(){return m_dataBuffer;}

    unsigned long long bufferSize() const;
    void appendBuffer(const long double &ldblTimeFrame, const long double &ldblValue);
    virtual stData bufferData(const unsigned long long &ullTFIndex);
    bool isBufferDataExist(const unsigned long long &ullTFIndex);

	void operator << (const stData &data)
	{
        appendBuffer(data.ldblTimeFrame, data.ldblValue);
	}

    void beforeCalc(const long double &ldblStartTime, const long double &ldblTimeStep,
                    const long double &ldblEndTime);
private slots:
    void setPortalOrientation(const int &portalOrientation);
public slots:
    virtual void updateGeometry();
signals:
	void dataColorChanged(QColor dataColor);
    void geometryChanged();
};

#endif // CPORTAL_H