#include "cxmlscheme.h"

#include <QMetaProperty>

#include "cscheme.h"
#include "celement.h"
#include "algorithm/calgorithm.h"
#include "link/clink.h"

QDomElement CXMLScheme::variantToDomElement(QDomDocument domDoc, const QString &name, const QVariant &value)
{
	if(name.isEmpty()) return QDomElement();
	if(!value.isValid() || !value.canConvert(QVariant::String)) return QDomElement();

	QDomElement domElement = domDoc.createElement(name);
	QDomText domText = domDoc.createTextNode(value.toString());
	domElement.appendChild(domText);

	return domElement;
}

QVariant CXMLScheme::domElementToVariant(const QDomElement &domElement)
{
	if(domElement.isNull()) return QVariant();
	return QVariant(domElement.text());
}

void CXMLScheme::writeElement(QDomDocument &doc, QDomElement &domElParent, CElement *element)
{
	if(!element) return;
	if(element->objectName().isEmpty()) return;

	QDomElement domElElement = doc.createElement("element");
	domElElement.setAttribute("typeID", element->typeID());
	domElElement.setAttribute("id", element->id());
	domElElement.setAttribute("nomber", QString("%1").arg(element->nomber()));
	domElElement.setAttribute("name", element->name());

	QString ct;
	ct = !element->parentItem() ? "create" : "setup";
	domElElement.setAttribute("ct", ct);

	QDomElement domElProps = doc.createElement("properties");
	for(int ci = 0; ci < element->metaObject()->propertyCount(); ++ci)
	{
		QMetaProperty prop = element->metaObject()->property(ci);
		if(prop.isValid() && prop.isReadable() && prop.isWritable() && (prop.userType() != QMetaType::UnknownType))
		{
			domElProps.appendChild(variantToDomElement(doc, prop.name(), element->property(prop.name())));
		}
	}
	domElElement.appendChild(domElProps);

	QDomElement domElChildren = doc.createElement("children");
	foreach(CElement *childElement, element->childElements())
	{
		writeElement(doc, domElChildren, childElement);
	}
	domElElement.appendChild(domElChildren);

	domElParent.appendChild(domElElement);
}

void CXMLScheme::readElement(CScheme *scheme, QDomDocument &domDoc, QDomElement &domElElement)
{
	if(!domElElement.isNull())
	{
		QString typeID = domElElement.attribute("typeID");
		QString id = domElElement.attribute("id");
		QString strNomber = domElElement.attribute("nomber");
		QString name = domElElement.attribute("name");
		QString ct = domElElement.attribute("ct");
		if(ct.isEmpty() || id.isEmpty() || typeID.isEmpty()) return;

		CElement *element = 0;
		if(ct == "create")
		{
			element = scheme->createElement(typeID);
			element->setName(name);
			element->setNomber(strNomber.toInt());
		}
		else if(ct == "setup")
		{
			element = scheme->element(id);
		}
		if(!element) return;

		QDomElement domElProps = domElElement.firstChildElement("properties");
		if(!domElProps.isNull())
		{
			QDomElement domElProp = domElProps.firstChildElement();
			while(!domElProp.isNull())
			{
				element->setProperty(qPrintable(domElProp.tagName()), domElementToVariant(domElProp));

				domElProp = domElProp.nextSiblingElement();
			}
		}

		QDomElement domElChildren = domElElement.firstChildElement("children");
		if(!domElChildren.isNull())
		{
			QDomElement domElChild = domElChildren.firstChildElement();
			while(!domElChild.isNull())
			{
				readElement(scheme, domDoc, domElChild);
				domElChild = domElChild.nextSiblingElement();
			}
		}
	}
}

CXMLScheme::CXMLScheme(QObject *parent) : QObject(parent)
{
	setObjectName(QStringLiteral("CXMLScheme"));
}

QDomDocument CXMLScheme::schemeToDom(CScheme *scheme)
{
	QDomDocument domDoc("scheme");
	if(!scheme) return domDoc;

	QDomProcessingInstruction instr = domDoc.createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'");
	domDoc.appendChild(instr);

	QDomElement domElBody = domDoc.createElement("body");

	QDomElement domElHeader = domDoc.createElement("scheme_header");
	domElHeader.appendChild(variantToDomElement(domDoc, "version", "1.0"));
	domElBody.appendChild(domElHeader);

	QDomElement domElAlgorithms = domDoc.createElement("algorithms");
	QList<CAlgorithm*> algs = scheme->algorithms();
	foreach(CAlgorithm *alg, algs)
	{
		writeElement(domDoc, domElAlgorithms, alg);
	}
	domElBody.appendChild(domElAlgorithms);

	QDomElement domElLinks = domDoc.createElement("links");
	QList<CLink*> links = scheme->links();
	foreach(CLink *link, links)
	{
		writeElement(domDoc, domElLinks, link);
	}
	domElBody.appendChild(domElLinks);

	domDoc.appendChild(domElBody);

	return domDoc;
}

void CXMLScheme::schemeFromDom(CScheme *scheme, QDomDocument domDoc)
{
	if(!scheme) return;

	QDomNode domNodeInstr = domDoc.firstChild();
	if(!domNodeInstr.isProcessingInstruction() || domNodeInstr.isNull()) return;
	QDomProcessingInstruction domProcInstr = domNodeInstr.toProcessingInstruction();
	if(domProcInstr.isNull()) return;
	if(domProcInstr.target() != "xml" || domProcInstr.data() != "version='1.0' encoding='UTF-8'") return;
	if(domDoc.doctype().name() != "scheme") return;
	QDomElement domElBody = domDoc.documentElement();
	if(domElBody.tagName() != "body") return;
	QDomElement domElHeader = domElBody.firstChildElement("scheme_header");
	if(domElHeader.isNull()) return;
	QDomElement domElVersion = domElHeader.firstChildElement("version");
	if(domElVersion.isNull()) return;
	QVariant varVersion = domElementToVariant(domElVersion);
	if(!varVersion.isValid() || !varVersion.canConvert(QVariant::String)) return;
	if(varVersion.toString() != "1.0") return;
	QDomElement domElAlgorithms = domElBody.firstChildElement("algorithms");
	if(domElAlgorithms.isNull()) return;

	QDomElement domElAlgorithm = domElAlgorithms.firstChildElement();
	while(!domElAlgorithm.isNull())
	{
		readElement(scheme, domDoc, domElAlgorithm);
		domElAlgorithm = domElAlgorithm.nextSiblingElement();
	}

	QDomElement domElLinks = domElBody.firstChildElement("links");
	if(domElLinks.isNull()) return;

	QDomElement domElLink = domElLinks.firstChildElement();
	while(!domElLink.isNull())
	{
		readElement(scheme, domDoc, domElLink);
		domElLink = domElLink.nextSiblingElement();
	}
}