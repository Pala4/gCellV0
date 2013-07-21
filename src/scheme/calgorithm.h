#ifndef CALGORITHM_H
#define CALGORITHM_H

#include <QGraphicsItem>

#include "celement.h"

class CScheme;
class CPortal;
class CArgument;
class CResult;

class CAlgorithm : public CElement
{
	Q_OBJECT
private:
	QList<CPortal*> m_portals;
	QRectF m_innerBound;
	QRectF m_boundingRect;
	void calcBounds(void);
	void placePortals(void);
protected:
	void addPortal(CPortal *portal);
	CArgument* addArgument(const QString &name);
	CResult* addResult(const QString &name);
public:
	explicit CAlgorithm(QGraphicsItem *parent = 0);

	virtual QRectF boundingRect(void) const{return m_boundingRect;}
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private slots:
	void onPortalDestroyed(QObject *objPortal);
public slots:
	virtual void updateGeometry(void);
};

#endif // CALGORITHM_H