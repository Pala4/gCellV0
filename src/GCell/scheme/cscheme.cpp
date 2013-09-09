#include "cscheme.h"

#include <QMetaProperty>

#include "algorithmproto/calgorithmproto.h"
#include "algorithmproto/calgorithmprotomng.h"
#include "algorithm/calgorithm.h"
#include "portal/cresult.h"
#include "portal/cargument.h"
#include "link/clink.h"
#include "elementlistutil.h"
#include "varutil.h"

QDomElement CScheme::variantToDomElement(QDomDocument domDoc, const QString &name, const QVariant &value)
{
	 if(name.isEmpty()) return QDomElement();
	 if(!value.isValid()) return QDomElement();

	 QDomElement domElement = domDoc.createElement(name);
	 QDomText domText = domDoc.createTextNode(variantToString(value));
	 domElement.appendChild(domText);

	 return domElement;
}

QVariant CScheme::domElementToVariant(const QDomElement &domElement)
{
	 if(domElement.isNull()) return QVariant();
	 return QVariant(stringToVariant(domElement.text()));
}

void CScheme::writeElementToXML(QDomDocument &doc, QDomElement &domElParent, CElement *element)
{
	 if(!element) return;
	 if(element->objectName().isEmpty()) return;

	 QDomElement domElElement = doc.createElement("element");
	 domElElement.setAttribute("typeID", element->typeID());
	 domElElement.setAttribute("id", element->id());

	 QString nodeType;
	 nodeType = !element->parentItem() ? "rootNode" : "subNode";
	 domElElement.setAttribute("nodeType", nodeType);

	 QDomElement domElProps = doc.createElement("properties");
	 for(int ci = 0; ci < element->metaObject()->propertyCount(); ++ci)
	 {
		  QMetaProperty prop = element->metaObject()->property(ci);
		  if(prop.isValid() &&prop.isReadable() && prop.isWritable() &&
			 (prop.userType() != QMetaType::UnknownType) && (prop.userType() != QMetaType::User) &&
			 (prop.userType() != qMetaTypeId<QGraphicsObject*>())) //fix for QGraphicsObject* 'as parent'
		  {
				domElProps.appendChild(variantToDomElement(doc, prop.name(), element->property(prop.name())));
		  }
	 }
	 domElElement.appendChild(domElProps);

	 QDomElement domElChildren = doc.createElement("children");
	 foreach(CElement *childElement, element->childElements())
	 {
		  writeElementToXML(doc, domElChildren, childElement);
	 }
	 domElElement.appendChild(domElChildren);

	 domElParent.appendChild(domElElement);
}

CElement* CScheme::readElementFromXML(const QDomDocument &domDoc, const QDomElement &domElElement, CElement *parentElement)
{
	 if(domElElement.isNull()) return 0;

	 QString typeID = domElElement.attribute("typeID");
	 QString id = domElElement.attribute("id");
	 QString nodeType = domElElement.attribute("nodeType");
	 if(nodeType.isEmpty() || id.isEmpty() || typeID.isEmpty()) return 0;

	 CElement *el = 0;
	 if(nodeType == "rootNode")
	 {
		  el = createElement(typeID);
	 }
	 else if((nodeType == "subNode") && parentElement)
	 {
		  el = parentElement->childElement(id);
		  if(!el) el = parentElement->createElement(typeID);
	 }
	 if(!el) return 0;

	 QDomElement domElProps = domElElement.firstChildElement("properties");
	 if(!domElProps.isNull())
	 {
		  QDomElement domElProp = domElProps.firstChildElement();
		  while(!domElProp.isNull())
		  {
				el->setProperty(qPrintable(domElProp.tagName()), domElementToVariant(domElProp));

				domElProp = domElProp.nextSiblingElement();
		  }
	 }

	 QDomElement domElChildren = domElElement.firstChildElement("children");
	 if(!domElChildren.isNull())
	 {
		  QDomElement domElChild = domElChildren.firstChildElement();
		  while(!domElChild.isNull())
		  {
				readElementFromXML(domDoc, domElChild, el);
				domElChild = domElChild.nextSiblingElement();
		  }
	 }

	 return el;
}

CScheme::CScheme(QObject *parent) : QGraphicsScene(parent)
{
	 setObjectName(QStringLiteral("CScheme"));

	 m_newScheme = false;
	 m_modified = false;
	 m_fileName = QStringLiteral("scheme.scm");

	 m_algProtoMng = 0;

	 connect(this, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));
}

void CScheme::setFileName(const QString &fileName)
{
	 if(m_fileName == fileName) return;
	 m_fileName = fileName;
	 emit fileNameChanged(m_fileName);
}

CElement* CScheme::createElement(const QString &typeID)
{
	 CElement *element = 0;
	 if(typeID == QStringLiteral("CLink"))
	 {
		  element = new CLink();
	 }
	 else
	 {
		  if(m_algProtoMng)
		  {
				CAlgorithmProto *algProto = m_algProtoMng->algorithmProto(typeID);
				if(algProto) element = algProto->createAlgorithm();
		  }
	 }
	 return element;
}

QList<CElement*> CScheme::elements(const QString &typeID)
{
	 return getElements<CElement*, QGraphicsItem*>(items(), typeID);
}

QList<CElement*> CScheme::selectedElements(void)
{
	 return getElements<CElement*, QGraphicsItem*>(selectedItems());
}

QList<CAlgorithm *> CScheme::selectedAlgorithms(void)
{
	 return getElements<CAlgorithm*, QGraphicsItem*>(selectedItems());
}

CElement* CScheme::element(const QString &id)
{
	 QList<CElement*> els = elements();
	 foreach(CElement *element, els)
	 {
		  if(element && element->id() == id) return element;
	 }
	 return 0;
}

QList<CAlgorithm*> CScheme::algorithms(void)
{
	 return getElements<CAlgorithm*, QGraphicsItem*>(items());
}

QList<CLink*> CScheme::links(void)
{
	 return getElements<CLink*, QGraphicsItem*>(items());
}

bool CScheme::checkXMLSchemeFormat(const QDomDocument &domDoc)
{
	 QDomNode domNodeInstr = domDoc.firstChild();
	 if(!domNodeInstr.isProcessingInstruction() || domNodeInstr.isNull()) return false;
	 QDomProcessingInstruction domProcInstr = domNodeInstr.toProcessingInstruction();
	 if(domProcInstr.isNull()) return false;
	 if(domProcInstr.target() != "xml" || domProcInstr.data() != "version='1.0' encoding='UTF-8'") return false;
	 if(domDoc.doctype().name() != "scheme") return false;
	 QDomElement domElBody = domDoc.documentElement();
	 if(domElBody.tagName() != "body") return false;
	 QDomElement domElHeader = domElBody.firstChildElement("scheme_header");
	 if(domElHeader.isNull()) return false;
	 QDomElement domElVersion = domElHeader.firstChildElement("version");
	 if(domElVersion.isNull()) return false;
	 QVariant varVersion = domElementToVariant(domElVersion);
	 if(!varVersion.isValid() || !varVersion.canConvert(QVariant::String)) return false;
	 if(varVersion.toString() != "1.0") return false;
	 QDomElement domElElements = domElBody.firstChildElement("elements");
	 if(domElElements.isNull()) return false;
	 return true;
}

QDomDocument CScheme::toXMLDom(const QList<CElement*> &elements, stSchemeDesc *schemeDesc)
{
	 QDomDocument domDoc("scheme");

	 QDomProcessingInstruction instr = domDoc.createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'");
	 domDoc.appendChild(instr);

	 QDomElement domElBody = domDoc.createElement("body");

	 QDomElement domElHeader = domDoc.createElement("scheme_header");
	 domElHeader.appendChild(variantToDomElement(domDoc, "version", "1.0"));
	 if(schemeDesc)
	 {
		  domElHeader.appendChild(variantToDomElement(domDoc, "width", schemeDesc->width));
		  domElHeader.appendChild(variantToDomElement(domDoc, "height", schemeDesc->height));
	 }
	 domElBody.appendChild(domElHeader);

	 QDomElement domElElements = domDoc.createElement("elements");
	 foreach(CElement *element, elements)
	 {
		  if(!element || element->parentItem()) continue;
		  writeElementToXML(domDoc, domElElements, element);
	 }
	 domElBody.appendChild(domElElements);

	 domDoc.appendChild(domElBody);

	 return domDoc;
}

QList<CElement*> CScheme::fromXMLDom(const QDomDocument &domDoc, stSchemeDesc *schemeDesc)
{
	 QList<CElement*> elements;
	 if(!checkXMLSchemeFormat(domDoc)) return elements;

	 if(schemeDesc)
	 {
		  qreal width = schemeDesc->width;
		  qreal height = schemeDesc->height;

		  QDomElement domElBody = domDoc.documentElement();
		  if(domElBody.tagName() == "body")
		  {
				QDomElement domElHeader = domElBody.firstChildElement("scheme_header");
				if(!domElHeader.isNull())
				{
					 QDomElement domElWidth = domElHeader.firstChildElement("width");
					 if(!domElWidth.isNull())
					 {
						  QVariant varWidth = domElementToVariant(domElWidth);
						  if(varWidth.isValid() && varWidth.canConvert(QVariant::Double))
						  {
								width = varWidth.toDouble();
						  }
					 }

					 QDomElement domElHeight = domElHeader.firstChildElement("height");
					 if(!domElHeight.isNull())
					 {
						  QVariant varHeight = domElementToVariant(domElHeight);
						  if(varHeight.isValid() && varHeight.canConvert(QVariant::Double))
						  {
								height = varHeight.toDouble();
						  }
					 }

					 schemeDesc->width = width;
					 schemeDesc->height = height;
				}
		  }
	 }

	 QDomElement domElBody = domDoc.documentElement();
	 QDomElement domElElements = domElBody.firstChildElement("elements");
	 QDomElement domElElement = domElElements.firstChildElement();
	 while(!domElElement.isNull())
	 {
		  CElement *element = readElementFromXML(domDoc, domElElement);
		  domElElement = domElElement.nextSiblingElement();
		  if(element) elements << element;
	 }

	 QList<CElement*> resultElements;
	 foreach(CElement *element, elements)
	 {
		  if(!element) continue;
		  {
				if(!element->reIndexing(elements))
				{
					 element->deleteLater();
				}
				else
				{
					 resultElements << element;
				}
		  }
	 }

	 return resultElements;
}

void CScheme::onSelectionChanged(void)
{
	emit algorithmsSelected(getElements<CAlgorithm*, QGraphicsItem*>(selectedItems()));
}

void CScheme::addElement(CElement *element)
{
	if(!element) return;

	addItem(element);

	emit elementAdded(element);
}

void CScheme::addElements(const QList<CElement*> &els)
{
	 foreach(CElement *el, els)
	 {
		  if(!el) continue;
		  if(element(el->id())) el->setNomber(generateNomber<CElement*, CElement*>(elements(el->typeID())));
		  addElement(el);
	 }
}

void CScheme::createAlgorithm(const QPointF &pos)
{
	 if(!m_algProtoMng) return;
	 addAlgorithm(m_algProtoMng->selectedAlgorithmProto(), pos);
}

void CScheme::addAlgorithm(CAlgorithmProto *algorithmProto, const QPointF &pos)
{
	 if(!algorithmProto) return;

	 CAlgorithm *algorithm = algorithmProto->createAlgorithm();
	 addAlgorithm(algorithm, pos);
}

void CScheme::addAlgorithm(CAlgorithm *algorithm, const QPointF &pos)
{
	 if(!algorithm) return;

	 algorithm->setNomber(generateNomber<CAlgorithm*, CAlgorithm*>(getElements<CAlgorithm*, QGraphicsItem*>(items(), algorithm->typeID())));
	 algorithm->setPos(pos);
	 addElement(algorithm);
}

void CScheme::addLink(const QString &firstPortalID, const QString &secondPortalID)
{
	 addLink(qobject_cast<CPortal*>(element(firstPortalID)), qobject_cast<CPortal*>(element(secondPortalID)));
}

void CScheme::addLink(CPortal *firstPortal, CPortal *secondPortal)
{
	 if(!firstPortal || !secondPortal) return;

	 CResult *result = 0;
	 CArgument *argument = 0;
	 if(qobject_cast<CResult*>(firstPortal))
	 {
		  result = qobject_cast<CResult*>(firstPortal);
		  argument = qobject_cast<CArgument*>(secondPortal);
	 }
	 else if(qobject_cast<CResult*>(secondPortal))
	 {
		  result = qobject_cast<CResult*>(secondPortal);
		  argument = qobject_cast<CArgument*>(firstPortal);
	 }
	 if(!result || !argument) return;

	 CLink *link = new CLink(0);
	 link->setResult(result);
	 link->setArgument(argument);
	 link->setNomber(generateNomber<CLink*, CLink*>(getElements<CLink*, QGraphicsItem*>(items())));
	 addElement(link);
	 link->updateGeometry();
}

void CScheme::deleteSelected(void)
{
	 QList<CElement*> selElements = selectedElements();
	 clearSelection();
	 foreach(CElement *selElement, selElements)
	 {
		  if(selElement && (selElement->intercations() & CElement::Deletable)) selElement->deleteLater();
	 }
}