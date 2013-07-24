#ifndef CELEMENT_H
#define CELEMENT_H

#include <QGraphicsObject>

class QAction;

class CScheme;

class CElement : public QGraphicsObject
{
    Q_OBJECT
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
	QString name(void) const;
	void setName(const QString &name);
	const QString& defaultName(void) const{return m_defaultName;}
	void setDefaultName(const QString &defaultName);
	const int& nomber(void) const{return m_nomber;}
	void setNomber(const int &nomber){m_nomber = nomber;}

	const QList<QAction*>& actions(void) const{return m_actions;}
	void addAction(QAction *action);

	virtual void calc(const int &timeFrame) = 0;
public slots:
	virtual void updateGeometry(void){}
signals:
	void nameChanged(QString name);
};

#endif // CELEMENT_H