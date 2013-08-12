#ifndef CELEMENT_H
#define CELEMENT_H

#include <QGraphicsObject>

class QAction;

class CScheme;
class CElementOptionsWgt;

class CElement : public QGraphicsObject
{
    Q_OBJECT
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
private:
	QString m_name;
	QString m_defaultName;
	int m_nomber;
	QList<QAction*> m_actions;
protected:
	CScheme* scheme(void);
public:
	explicit CElement(QGraphicsItem *parent = 0);

	QString typeID(void){return metaObject()->className();}

	QString id(void);
	const QString& name(void) const{return m_name;}
	void setName(const QString &name);
	const QString& defaultName(void) const{return m_defaultName;}
	void setDefaultName(const QString &defaultName);
	const int& nomber(void) const{return m_nomber;}
	void setNomber(const int &nomber){m_nomber = nomber;}

	QString caption(void) const;

	const QList<QAction*>& actions(void) const{return m_actions;}
	void addAction(QAction *action);

	virtual CElement* createElement(const QString &typeID){Q_UNUSED(typeID) return 0;}
	CElement* childElement(const QString &id);
	QList<CElement*> childElements(void);

	virtual CElementOptionsWgt* optionsWidget(QWidget *parentWidget);
	virtual void acceptOptions(CElementOptionsWgt *optWgt);

	virtual bool reIndexing(const QList<CElement*> &elements){Q_UNUSED(elements) return true;}
    virtual void beforeCalc(void){}
	virtual void calc(const int &timeFrame) = 0;
    virtual void afterCalc(void){}
public slots:
	virtual void updateGeometry(void){}
signals:
	void nameChanged(QString name);
};

#endif // CELEMENT_H