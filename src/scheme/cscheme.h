#ifndef CSCHEME_H
#define CSCHEME_H

#include <QGraphicsScene>

class QAction;

class CElement;
class CAlgorithmProto;
class CAlgorithmProtoMng;
class CAlgorithm;
class CPortal;

class CScheme : public QGraphicsScene
{
    Q_OBJECT
private:
	QAction *m_acDeleteSelected;
	QList<QAction*> m_actions;
    CAlgorithmProtoMng *m_algProtoMng;
public:
    explicit CScheme(QObject *parent = 0);

	QString id(void){return QString("Scheme");}

	const QList<QAction*>& actions(void){return m_actions;}
	void addAction(QAction *action);
    void setAlgorithmProtoMng(CAlgorithmProtoMng *algorithmProtoMng){m_algProtoMng = algorithmProtoMng;}
    CElement* element(const QString &id);
private slots:
	void onSelectionChanged(void);
public slots:
    void createAlgorithm(const QPointF &pos);
	void addAlgorithm(CAlgorithmProto *algorithmProto, const QPointF &pos);
	void addAlgorithm(CAlgorithm *algorithm, const QPointF &pos);
	void addLink(CPortal *firstPortal, CPortal *secondPortal);
	void deleteSelected(void);
};

#endif // CSCHEME_H