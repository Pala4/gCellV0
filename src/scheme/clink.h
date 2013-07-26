#ifndef CLINK_H
#define CLINK_H

#include "celement.h"

class CPortal;

class CLink : public CElement
{
    Q_OBJECT
	Q_PROPERTY(QString argumentID READ argumentID WRITE setArgumentID)
	Q_PROPERTY(QString resultID READ resultID WRITE setResultID)
private:
	CPortal *m_result;
	CPortal *m_argument;
	QPainterPath m_path;

    static QPainterPath shapeFromPath(const QPainterPath &path, const QPen &pen);
	void calcLink(void);
public:
	explicit CLink(QGraphicsItem *parent = 0);

	virtual QPainterPath shape(void) const;
	virtual QRectF boundingRect(void) const;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	QString argumentID(void);
	void setArgumentID(const QString &argumentID);
	QString resultID(void);
	void setResultID(const QString &resultID);

	CPortal* result(void){return m_result;}
	CPortal* argument(void){return m_argument;}
	void setResult(CPortal *result){m_result = result; updateGeometry();}
	void setArgument(CPortal *argument){m_argument = argument; updateGeometry();}

	virtual void calc(const int &timeFrame);
public slots:
	virtual void updateGeometry(void);
};

#endif // CLINK_H