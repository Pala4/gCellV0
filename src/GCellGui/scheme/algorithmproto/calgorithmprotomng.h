#ifndef CALGORITHMPROTOMNG_H
#define CALGORITHMPROTOMNG_H

#include <QObject>

#include <QList>

class CAlgorithmProto;

class CAlgorithmProtoMng : public QObject
{
    Q_OBJECT
private:
	CAlgorithmProto *m_selectedAlgorithmProto;
    QList<CAlgorithmProto*> m_algorithmProtos;
public:
	explicit CAlgorithmProtoMng(QObject *parent = 0);

    QList<CAlgorithmProto*> algorithmProtos(void) const{return m_algorithmProtos;}
	CAlgorithmProto* algorithmProto(const QString &id);
	CAlgorithmProto* addProto(const QString &name, const QString &groupName,
							  const QMetaObject *algorithmMO);
	CAlgorithmProto* selectedAlgorithmProto(void){return m_selectedAlgorithmProto;}
public slots:
	void setSelectedAlgorithmProto(CAlgorithmProto *algorithmProto);
signals:
	void algorithmProtoAdded(CAlgorithmProto *algorithmProto);
	void algorithmProtoRemoved(CAlgorithmProto *algorithmProto);
	void algorithmProtoSelected(CAlgorithmProto *algorithmProto);
};

#endif // CALGORITHMPROTOMNG_H
