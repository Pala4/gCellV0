#ifndef CLINK_H
#define CLINK_H

#include "../celement.h"

class CPortal;
class CResult;
class CArgument;

class CLink : public CElement
{
    Q_OBJECT
	Q_PROPERTY(QString resultID READ resultID WRITE setResultID)
	Q_PROPERTY(QString argumentID READ argumentID WRITE setArgumentID)
private:
	QString m_resultID;
	QString m_argumentID;
	CResult *m_result;
	CArgument *m_argument;
	QPainterPath m_path;
	qreal m_distFromPortal;

    static QPainterPath shapeFromPath(const QPainterPath &path, const QPen &pen);
	void calcLink(void);
protected:
	virtual QRectF calcBounds(void);
public:
	explicit CLink(QGraphicsItem *parent = 0);
	virtual ~CLink(void);

	virtual QPainterPath shape(void) const;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	QString resultID(void);
	void setResultID(const QString &resultID);
	QString argumentID(void);
	void setArgumentID(const QString &argumentID);

	CResult* result(void){return m_result;}
	CArgument* argument(void){return m_argument;}
	void setResult(CResult *result);
	void setArgument(CArgument *argument);

	virtual bool reIndexing(const QList<CElement*> &elements);

	virtual void calc(const int &timeFrame);
private slots:
	void onResultDataColorChanged(const QColor &dataColor);
	void onResultDestroyed(QObject *objResult);
	void onArgumentDestroyed(QObject *objArgument);
public slots:
	virtual void updateGeometry(void);
};

#endif // CLINK_H