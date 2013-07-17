#ifndef CELEMENTPROTO_H
#define CELEMENTPROTO_H

#include <QObject>

class CElement;

class CElementProto : public QObject
{
    Q_OBJECT
private:
    QString m_id;
    QString m_name;
    const QMetaObject *m_elementMO;
public:
    explicit CElementProto(QObject *parent = 0);
    explicit CElementProto(const QString &id, const QString &name, const QMetaObject *elementMO, QObject *parent = 0);

    const QString& id(void) const{return m_id;}
    void setID(const QString &id){m_id = id;}
    const QString& name(void) const{return m_name;}
    void setName(const QString &name){m_name = name;}
    const QMetaObject* elementMO(void){return m_elementMO;}
    void setElementMO(const QMetaObject *elementMO){m_elementMO = elementMO;}

    static bool checkElementMO(const QMetaObject *elementMO);

    CElement* createElement(void);
};

#endif // CELEMENTPROTO_H