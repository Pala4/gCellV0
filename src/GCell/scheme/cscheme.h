#ifndef CSCHEME_H
#define CSCHEME_H

#include <QGraphicsScene>

#include <QtXml/QDomDocument>

class CElement;
class CAlgorithmProto;
class CAlgorithmProtoMng;
class CAlgorithm;
class CPortal;
class CLink;

class CScheme : public QGraphicsScene
{
    Q_OBJECT
private:
	bool m_newScheme;
	bool m_modified;
    CAlgorithmProtoMng *m_algProtoMng;

	QDomElement variantToDomElement(QDomDocument domDoc, const QString &name, const QVariant &value);
	QVariant domElementToVariant(const QDomElement &domElement);
	void writeElementToXML(QDomDocument &doc, QDomElement &domElParent, CElement *element);
	CElement* readElementFromXML(const QDomDocument &domDoc, const QDomElement &domElElement, CElement *parentElement = 0);
public:
    explicit CScheme(QObject *parent = 0);

	QString id(void){return QString("Scheme");}
	const bool& isNewScheme(void) const{return m_newScheme;}
	void setNewScheme(const bool &newScheme){m_newScheme = newScheme;}
	const bool& isModified(void) const{return m_modified;}
	void setModified(const bool &modified){m_modified = modified;}

    void setAlgorithmProtoMng(CAlgorithmProtoMng *algorithmProtoMng){m_algProtoMng = algorithmProtoMng;}
	CElement* createElement(const QString &typeID);
	QList<CElement*> elements(const QString &typeID = QString());
	QList<CElement*> selectedElements(void);
	CElement* element(const QString &id);
	QList<CAlgorithm*> algorithms(void);
	QList<CLink*> links(void);

	bool checkXMLSchemeFormat(const QDomDocument &domDoc);
	QDomDocument toXMLDom(const QList<CElement*> &elements);
	QList<CElement*> fromXMLDom(const QDomDocument &domDoc);
private slots:
	void onSelectionChanged(void);
public slots:
	void addElements(const QList<CElement*> &els);
    void createAlgorithm(const QPointF &pos);
	void addAlgorithm(CAlgorithmProto *algorithmProto, const QPointF &pos);
	void addAlgorithm(CAlgorithm *algorithm, const QPointF &pos);
	void addLink(const QString &firstPortalID, const QString &secondPortalID);
	void addLink(CPortal *firstPortal, CPortal *secondPortal);
	void deleteSelected(void);
signals:
	void algorithmsSelected(QList<CAlgorithm*> algorithms);
};

#endif // CSCHEME_H