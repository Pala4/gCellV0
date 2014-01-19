#ifndef CELEMENT_H
#define CELEMENT_H

#include <QGraphicsTextItem>
#include <QGraphicsObject>

#include "cobjectitem.h"

#include <QFont>

#include "timeframe.h"

class QAction;
class QGraphicsTextItem;

class CScheme;
class CElementOptionsWgt;
class CGrid;

class CGraphicsTextItem : public QGraphicsTextItem, public CObjectItem
{
	Q_OBJECT
protected:
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
public:
	explicit CGraphicsTextItem(const QString &text, QGraphicsItem *parent = 0);

    QObject* parentObject();
    QObjectList childrenObjects();
private slots:
	void onDocumentContentChanged(void);
public slots:
	void setText(const QString &text);
signals:
	void textChanged(const QString &text);
};

class CElement : public QGraphicsObject, public CObjectItem
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
	QFont m_captionFont;
	CElement::Interactions m_intercations;

	QRectF m_boundingRect;

	bool m_blockCaptionEditorSetText;
	CGraphicsTextItem *m_captionEditor;
	QList<QAction*> m_actions;

	CGrid *m_grid;
protected:
	friend class CGraphicsTextItem;
	CScheme* scheme(void);
	void setInteractions(const CElement::Interactions &interactions){m_intercations = interactions;}
	CGraphicsTextItem* captionEditor(void){return m_captionEditor;}
	virtual QRectF calcBounds(void);
	virtual QPointF captionEditorPosition(void){return QPointF(0.0, 0.0);}
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
public:
	explicit CElement(QGraphicsItem *parent = 0);

	virtual QRectF boundingRect(void) const{return m_boundingRect;}
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QObject* parentObject();
    QObjectList childrenObjects();

	QString typeID(void){return metaObject()->className();}

	QString id(void);
	const QString& name(void) const{return m_name;}
	void setName(const QString &name);
	const QString& defaultName(void) const{return m_defaultName;}
	void setDefaultName(const QString &defaultName);
	const int& nomber(void) const{return m_nomber;}
	void setNomber(const int &nomber){m_nomber = nomber; emit nameChanged(name());}
	bool isCaptionVisible(void) const{return m_captionEditor ? m_captionEditor->isVisible() : false;}
	void setCaptionVisible(const bool &captionVisible){if(m_captionEditor) m_captionEditor->setVisible(captionVisible);}
	const QFont& captionFont(void) const{return m_captionFont;}
	void setCaptionFont(const QFont &captionFont);
	const CElement::Interactions& intercations(void) const{return m_intercations;}

    QString caption();

	const QList<QAction*>& actions(void) const{return m_actions;}
	void addAction(QAction *action);

	CGrid* grid(void){return m_grid;}
	void setGrid(CGrid *grid){m_grid = grid;}

	virtual CElement* createElement(const QString &typeID){Q_UNUSED(typeID) return 0;}
	CElement* childElement(const QString &id);
	QList<CElement*> childElements(void);

	virtual CElementOptionsWgt* optionsWidget(QWidget *parentWidget);
	virtual void acceptOptions(CElementOptionsWgt *optWgt);

	virtual bool reIndexing(const QList<CElement*> &elements){Q_UNUSED(elements) return true;}
	virtual void beforeCalc(const qreal &startTime, const qreal &timeStep, const qreal &endTime);
	virtual void calc(const stTimeLine &timeLine) = 0;
	virtual void afterCalc(void){}
private slots:
	void onCaptionEditorTextChanged(const QString &text);
	void onNameChanged(const QString &name);
public slots:
	virtual void updateGeometry(void);
signals:
	void nameChanged(QString name);
	void modified(void);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(CElement::Interactions)

#endif // CELEMENT_H