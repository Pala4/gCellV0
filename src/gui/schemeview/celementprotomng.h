#ifndef CELEMENTPROTOMNG_H
#define CELEMENTPROTOMNG_H

#include <QObject>

#include <QMap>

class CElementProto;

class CElementProtoMng : public QObject
{
    Q_OBJECT
private:
	CElementProto *m_selectedElementProto;
    QMap<QString, CElementProto*> m_elementProtos;
public:
    explicit CElementProtoMng(QObject *parent = 0);

    const QMap<QString, CElementProto*>& elementProtos(void) const{return m_elementProtos;}
    CElementProto* addProto(const QString &id, const QString &name, const QMetaObject *elementMO);
	void removeProto(const QString &id);
	CElementProto* selectedElementProto(void){return m_selectedElementProto;}
public slots:
	void selectElementProto(const QString &id);
	void setSelectedElementProto(CElementProto *elementProto);
signals:
    void elementProtoAdded(CElementProto *elementProto);
	void elementProtoRemoved(CElementProto *elementProto);
	void elementProtoSelected(CElementProto *elementProto);
};

#endif // CELEMENTPROTOMNG_H