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
    virtual QRectF calcBounds();
    void placePortals();
protected:
	void removePortal(const QString &id);
	void addArgument(CArgument *arg);
	CArgument* addArgument(const QString &name);
	void addResult(CResult *res);
	CResult* addResult(const QString &name);
    void clearResults();

    virtual void proced(const unsigned long long &ullTFIndex, const long double &ldblTimeFrame,
                        const long double &ldblStartTime, const long double &ldblTimeStep,
                        const long double &ldblEndTime) = 0;
public:
	explicit CAlgorithm(QGraphicsItem *parent = 0);

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	CResult* result(const QString &id);
    QList<CPortal*> portals();
    QList<CPortal*> argPortals();
    QList<CPortal*> resPortals();
    QList<CArgument*> arguments(){return m_arguments.values();}
    QList<CResult*> results(){return m_results.values();}

	virtual CElement* createElement(const QString &typeID);

    void calc(const unsigned long long &ullTFIndex, const long double &ldblTimeFrame,
              const long double &ldblStartTime, const long double &ldblTimeStep,
              const long double &ldblEndTime);
private slots:
	void onPortalDestroyed(QObject *objPortal);
public slots:
    virtual void updateGeometry();
signals:
	void portalAdded(CPortal *portal);
    void portalRemoved(QObject *objPortal);
};

#endif // CALGORITHM_H