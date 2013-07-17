#ifndef CELEMENT_H
#define CELEMENT_H

#include <QObject>

#include <QRectF>

class CElement : public QObject
{
    Q_OBJECT
private:
	QString m_typeID;
	int m_nomber;
	QString m_defaultName;
	QString m_name;
	QRectF m_geometry;
public:
	Q_INVOKABLE explicit CElement(QObject *parent = 0);

	const QString& typeID(void) const{return m_typeID;}
	void setTypeID(const QString &typeID){m_typeID = typeID;}
	const int& nomber(void) const{return m_nomber;}
	void setNomber(const int &nomber){m_nomber = nomber;}
	const QString& defaultName(void) const{return m_defaultName;}
	void setDefaultName(const QString &defaultName){m_defaultName = defaultName;}
	const QString& name(void) const{return m_name;}
	void setName(const QString &name){if(m_name == name) return; m_name = name; emit nameChanged(m_name);}
	const QRectF& geometry(void) const{return m_geometry;}
	void setGeometry(const QRectF &geometry){if(m_geometry == geometry) return; m_geometry = geometry; emit geometryChanged(m_geometry);}
	QPointF pos(void){return m_geometry.topLeft();}
	void setPos(const QPointF &pos){QRectF geom = m_geometry; geom.setTopLeft(pos); setGeometry(geom);}
	QSizeF size(void){return m_geometry.size();}
	void setSize(const QSizeF &size){QRectF geom = m_geometry; geom.setSize(size); setGeometry(geom);}

	QString id(void);
public slots:
	void resetName(void);
signals:
	void nameChanged(QString name);
	void geometryChanged(QRectF geometry);
};

#endif // CELEMENT_H