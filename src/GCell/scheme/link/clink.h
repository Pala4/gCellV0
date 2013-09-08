#ifndef CLINK_H
#define CLINK_H

#include "../celement.h"
#include "../portal/cportal.h"

class CResult;
class CArgument;
class CLinkEnv;
class CLinkSegments;

class CLink : public CElement
{
    Q_OBJECT
	Q_PROPERTY(QString resultID READ resultID WRITE setResultID)
	Q_PROPERTY(QString argumentID READ argumentID WRITE setArgumentID)
	Q_PROPERTY(QPointF firstOffset READ firstOffset WRITE setFirstOffset)
	Q_PROPERTY(QPointF medOffset READ medOffset WRITE setMedOffset)
	Q_PROPERTY(QPointF secondOffset READ secondOffset WRITE setSecondOffset)
private:
	QString m_resultID;
	QString m_argumentID;
	CResult *m_result;
	CArgument *m_argument;
	QPainterPath m_path;
	qreal m_distFromPortal;

	CLinkEnv *m_linkEnv;
	CLinkSegments *m_linkSegments;

    static QPainterPath shapeFromPath(const QPainterPath &path, const QPen &pen);
	void calcLink(void);
protected:
	virtual QRectF calcBounds(void);
public:
	explicit CLink(QGraphicsItem *parent = 0);
	virtual ~CLink(void);

	const qreal& distFromPortal(void) const{return m_distFromPortal;}
	void setDistFromPortal(const qreal &distFromPortal);
	QPointF firstPos(void);
	CPortal::TPortalOrientation firstOrientation(void);
	QRectF firstFigure(void);
	QPointF secondPos(void);
	CPortal::TPortalOrientation secondOrientation(void);
	QRectF secondFigure(void);

	virtual QPainterPath shape(void) const;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	QString resultID(void);
	void setResultID(const QString &resultID);
	QString argumentID(void);
	void setArgumentID(const QString &argumentID);

	const QPointF firstOffset(void) const;
	void setFirstOffset(const QPointF &firstOffset);
	const QPointF medOffset(void) const;
	void setMedOffset(const QPointF &medOffset);
	const QPointF secondOffset(void) const;
	void setSecondOffset(const QPointF &secondOffset);

	CResult* result(void){return m_result;}
	CArgument* argument(void){return m_argument;}
	void setResult(CResult *result);
	void setArgument(CArgument *argument);

	QList<CPortal*> portals(void);

	CLinkEnv* linkEnv(void){return m_linkEnv;}
	CLinkSegments* linkSegments(void){return m_linkSegments;}

	virtual bool reIndexing(const QList<CElement*> &elements);

	virtual void calc(const stTimeLine &timeLine);
private slots:
	void onResultDataColorChanged(const QColor &dataColor);
	void onResultDestroyed(QObject *objResult);
	void onArgumentDestroyed(QObject *objArgument);
public slots:
	virtual void updateGeometry(void);
signals:
	void formChanged(void);
};
Q_DECLARE_METATYPE(QPointF)

#endif // CLINK_H