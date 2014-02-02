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
    QRectF calcBounds();
    QPointF captionEditorPosition();
public:
    explicit CLink(QGraphicsItem *parent = nullptr);
    virtual ~CLink();

    const qreal& distFromPortal() const{return m_distFromPortal;}
	void setDistFromPortal(const qreal &distFromPortal);
    QPointF firstPos();
    CPortal::TPortalOrientation firstOrientation();
    QRectF firstFigure();
    QPointF secondPos();
    CPortal::TPortalOrientation secondOrientation();
    QRectF secondFigure();

    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QString resultID();
	void setResultID(const QString &resultID);
    QString argumentID();
	void setArgumentID(const QString &argumentID);

    const QPointF firstOffset() const;
	void setFirstOffset(const QPointF &firstOffset);
    const QPointF medOffset() const;
	void setMedOffset(const QPointF &medOffset);
    const QPointF secondOffset() const;
	void setSecondOffset(const QPointF &secondOffset);

    CResult* result(){return m_result;}
    CArgument* argument(){return m_argument;}
	void setResult(CResult *result);
	void setArgument(CArgument *argument);

    QList<CPortal*> portals();

    CLinkEnv* linkEnv(){return m_linkEnv;}
    CLinkSegments* linkSegments(){return m_linkSegments;}

    bool reIndexing(const QList<CElement*> &elements);

    void calc(const unsigned long long &ullTFIndex, const long double &ldblTimeFrame,
              const long double &ldblStartTime, const long double &ldblTimeStep,
              const long double &ldblEndTime);
private slots:
	void onResultDataColorChanged(const QColor &dataColor);
    void onResultDestroyed();
    void onArgumentDestroyed();
public slots:
    void updateGeometry();
signals:
    void formChanged();
};
Q_DECLARE_METATYPE(QPointF)

#endif // CLINK_H