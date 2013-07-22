#ifndef CLINK_H
#define CLINK_H

#include "celement.h"

class CPortal;

class CLink : public CElement
{
    Q_OBJECT
private:
    CPortal *m_firstPortal;
	CPortal *m_secondPortal;
	QPainterPath m_path;

    static QPainterPath shapeFromPath(const QPainterPath &path, const QPen &pen);
    void calc(void);
public:
	explicit CLink(QGraphicsItem *parent = 0);

	virtual QPainterPath shape(void) const;
	virtual QRectF boundingRect(void) const;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	void setFirstPortal(CPortal *firstPortal){m_firstPortal = firstPortal; updateGeometry();}
    void setSecondPortal(CPortal *secondPortal){m_secondPortal = secondPortal; updateGeometry();}
public slots:
	virtual void updateGeometry(void);
};

#endif // CLINK_H