#ifndef CALGORITHMPROTO_H
#define CALGORITHMPROTO_H

#include <QObject>

class CAlgorithm;

class CAlgorithmProto : public QObject
{
    Q_OBJECT
private:
    QString m_name;
	QString m_groupName;
	const QMetaObject *m_algorithmMO;
public:
	explicit CAlgorithmProto(QObject *parent = 0);
	explicit CAlgorithmProto(const QString &name, const QString &groupName,
							 const QMetaObject *algorithmMO, QObject *parent = 0);

    QString id(void) const{return m_algorithmMO != 0 ? m_algorithmMO->className() : QString();}
    const QString& name(void) const{return m_name;}
    void setName(const QString &name){m_name = name;}
	const QString& groupName(void) const{return m_groupName;}
	void setGroupName(const QString &groupName){m_groupName = groupName;}
	const QMetaObject* algorithmMO(void){return m_algorithmMO;}
	void setAlgorithmMO(const QMetaObject *algorithmMO){m_algorithmMO = algorithmMO;}

	static bool checkAlgorithmMO(const QMetaObject *algorithmMO);

	CAlgorithm* createAlgorithm(void);
};

#endif // CALGORITHMPROTO_H
