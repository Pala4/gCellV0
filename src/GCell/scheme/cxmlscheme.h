#ifndef CXMLSCHEME_H
#define CXMLSCHEME_H

#include <QObject>

#include <QtXml/QDomDocument>
#include <QVariant>

class CScheme;
class CElement;

class CXMLScheme : public QObject
{
	Q_OBJECT
private:
	QDomElement variantToDomElement(QDomDocument domDoc, const QString &name, const QVariant &value);
	QVariant domElementToVariant(const QDomElement &domElement);
	void writeElement(QDomDocument &doc, QDomElement &domElParent, CElement *element);
	void readElement(CScheme *scheme, QDomDocument &domDoc, QDomElement &domElElement);
public:
	explicit CXMLScheme(QObject *parent = 0);

	QDomDocument schemeToDom(CScheme *scheme);
	void schemeFromDom(CScheme *scheme, QDomDocument domDoc);
};

#endif // CXMLSCHEME_H