#ifndef CGRELEMENT_H
#define CGRELEMENT_H

#include <QGraphicsObject>

class QAction;

class CElement;

class CGrElement : public QGraphicsObject
{
    Q_OBJECT
private:
    QList<QAction*> m_actions;
    QAction *m_acDelete;
    CElement *m_element;
protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
public:
	explicit CGrElement(CElement *element, QGraphicsItem *parent = 0);

	virtual QRectF boundingRect(void) const;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void addAction(QAction *action);

	CElement* element(void){return m_element;}
	void setElement(CElement *element);
private slots:
	void onElementStateChanged(void);
};

#endif // CGRELEMENT_H