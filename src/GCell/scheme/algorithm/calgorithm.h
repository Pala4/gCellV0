#ifndef CALGORITHM_H
#define CALGORITHM_H

#include <QGraphicsItem>

#include "../celement.h"

class CScheme;
class CPortal;
class CArgument;
class CResult;

class CAlgorithm : public CElement
{
	Q_OBJECT
private:
	QMap<QString, CArgument*> m_arguments;
	QMap<QString, CResult*> m_results;
	qreal m_portalSpace;
	qreal m_portalMargin;

	void registerPortal(CPortal *portal);
	virtual QRectF calcBounds(void);
	void placePortals(void);
protected:
	void removePortal(const QString &id);
	CArgument* addArgument(const QString &name);
	CResult* addResult(const QString &name);
	void clearResults(void);

	virtual void proced(const stTimeLine &timeLine) = 0;
public:
	explicit CAlgorithm(QGraphicsItem *parent = 0);

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	CResult* result(const QString &id);
	QList<CPortal*> portals(void);
	QList<CPortal*> argPortals(void);
	QList<CPortal*> resPortals(void);
	QList<CArgument*> arguments(void){return m_arguments.values();}
	QList<CResult*> results(void){return m_results.values();}

	virtual CElement* createElement(const QString &typeID);

	virtual void calc(const stTimeLine &timeLine);
private slots:
	void onPortalDestroyed(QObject *objPortal);
public slots:
	virtual void updateGeometry(void);
signals:
	void portalAdded(CPortal *portal);
};

#endif // CALGORITHM_H