#ifndef CSCHEME_H
#define CSCHEME_H

#include <QGraphicsScene>

class QAction;

class CElement;
class CAlgorithmProto;
class CAlgorithm;
class CPortal;

class CScheme : public QGraphicsScene
{
    Q_OBJECT
private:
	QAction *m_acDeleteSelected;
	QList<QAction*> m_actions;
public:
    explicit CScheme(QObject *parent = 0);

	QString id(void){return QString("Scheme");}

//	template<typename ElementType>
//	QList<ElementType> getElements(const QString &typeID = QString())
//	{
//		QList<ElementType> elementList;
//		foreach(QGraphicsItem *item, items())
//		{
//			ElementType element = dynamic_cast<ElementType>(item);
//			if(element && (typeID.isEmpty() || (element->typeID() == typeID))) elementList << element;
//		}
//		return elementList;
//	}

	const QList<QAction*>& actions(void){return m_actions;}
	void addAction(QAction *action);
private slots:
	void onSelectionChanged(void);
public slots:
	void addAlgorithm(CAlgorithmProto *algorithmProto, const QPointF &pos);
	void addAlgorithm(CAlgorithm *algorithm, const QPointF &pos);
	void addLink(CPortal *firstPortal, CPortal *secondPortal);
	void deleteSelected(void);
};

#endif // CSCHEME_H