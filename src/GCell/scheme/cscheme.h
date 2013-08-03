#ifndef CSCHEME_H
#define CSCHEME_H

#include <QGraphicsScene>

class QAction;

class CElement;
class CAlgorithmProto;
class CAlgorithmProtoMng;
class CAlgorithm;
class CPortal;
class CLink;

class CScheme : public QGraphicsScene
{
    Q_OBJECT
private:
	bool m_newScheme;
	bool m_modified;
	QAction *m_acDeleteSelected;
	QList<QAction*> m_actions;
    CAlgorithmProtoMng *m_algProtoMng;
public:
    explicit CScheme(QObject *parent = 0);

	QString id(void){return QString("Scheme");}
	const bool& isNewScheme(void) const{return m_newScheme;}
	void setNewScheme(const bool &newScheme){m_newScheme = newScheme;}
	const bool& isModified(void) const{return m_modified;}
	void setModified(const bool &modified){m_modified = modified;}

	const QList<QAction*>& actions(void){return m_actions;}
	void addAction(QAction *action);
    void setAlgorithmProtoMng(CAlgorithmProtoMng *algorithmProtoMng){m_algProtoMng = algorithmProtoMng;}
	CElement* createElement(const QString &typeID);
	CElement* element(const QString &id);
	QList<CAlgorithm*> algorithms(void);
	QList<CLink*> links(void);
private slots:
	void onSelectionChanged(void);
public slots:
    void createAlgorithm(const QPointF &pos);
	void addAlgorithm(CAlgorithmProto *algorithmProto, const QPointF &pos);
	void addAlgorithm(CAlgorithm *algorithm, const QPointF &pos);
	void addLink(const QString &firstPortalID, const QString &secondPortalID);
	void addLink(CPortal *firstPortal, CPortal *secondPortal);
	void deleteSelected(void);
signals:
	void algorithmsSelected(QList<CAlgorithm*> algorithms);
};

#endif // CSCHEME_H