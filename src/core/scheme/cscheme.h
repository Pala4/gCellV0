#ifndef CSCHEME_H
#define CSCHEME_H

#include <QObject>

#include <QList>

class CElement;
class CLink;

class CScheme : public QObject
{
    Q_OBJECT
private:
    QList<CElement*> m_elements;
    QList<CLink*> m_links;
public:
    explicit CScheme(QObject *parent = 0);

    const QList<CElement*>& elements(void) const{return m_elements;}
    void addElement(CElement *element);

    const QList<CLink*>& links(void) const{return m_links;}
    void addLink(CElement *inElement, CElement *outElement);
signals:
	void elementAdded(CElement *element);
};

#endif // CSCHEME_H