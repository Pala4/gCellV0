#ifndef CELEMENT_H
#define CELEMENT_H

#include <QGraphicsObject>

#include <QFont>

#include "timeframe.h"

class QAction;

class CScheme;
class CElementOptionsWgt;

class CElement : public QGraphicsObject
{
	Q_OBJECT
	Q_PROPERTY(int nomber READ nomber WRITE setNomber)
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
	Q_PROPERTY(bool captionVisible READ isCaptionVisible WRITE setCaptionVisible)
public:
	enum Interaction{NoIntercations = 0x0, Copyable = 0x1, Deletable = 0x2, AllIntercations = 0xF};
	Q_DECLARE_FLAGS(Interactions, Interaction)
private:
	QString m_name;
	QString m_defaultName;
	int m_nomber;
	bool m_captionVisible;
	QFont m_captionFont;
	CElement::Interactions m_intercations;

	QRectF m_boundingRect;

	QList<QAction*> m_actions;
protected:
	CScheme* scheme(void);
	void setInteractions(const CElement::Interactions &interactions){m_intercations = interactions;}
	virtual QRectF calcBounds(void){return QRectF();}
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
public:
	explicit CElement(QGraphicsItem *parent = 0);

	virtual QRectF boundingRect(void) const{return m_boundingRect;}
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	QString typeID(void){return metaObject()->className();}

	QString id(void);
	const QString& name(void) const{return m_name;}
	void setName(const QString &name);
	const QString& defaultName(void) const{return m_defaultName;}
	void setDefaultName(const QString &defaultName);
	const int& nomber(void) const{return m_nomber;}
	void setNomber(const int &nomber){m_nomber = nomber;}
	const bool& isCaptionVisible(void) const{return m_captionVisible;}
	void setCaptionVisible(const bool &captionVisible){m_captionVisible = captionVisible; updateGeometry();}
	const QFont& captionFont(void) const{return m_captionFont;}
	void setCaptionFont(const QFont &captionFont);
	const CElement::Interactions& intercations(void) const{return m_intercations;}

	virtual QString caption(void) const;

	const QList<QAction*>& actions(void) const{return m_actions;}
	void addAction(QAction *action);

	virtual CElement* createElement(const QString &typeID){Q_UNUSED(typeID) return 0;}
	CElement* childElement(const QString &id);
	QList<CElement*> childElements(void);

	virtual CElementOptionsWgt* optionsWidget(QWidget *parentWidget);
	virtual void acceptOptions(CElementOptionsWgt *optWgt);

	virtual bool reIndexing(const QList<CElement*> &elements){Q_UNUSED(elements) return true;}
	virtual void beforeCalc(const qreal &startTime, const qreal &timeStep, const qreal &endTime);
	virtual void calc(const stTimeLine &timeLine) = 0;
	virtual void afterCalc(void){}
public slots:
	virtual void updateGeometry(void){m_boundingRect = calcBounds();}
signals:
	void nameChanged(QString name);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(CElement::Interactions)

#endif // CELEMENT_H