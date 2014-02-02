#ifndef CSCHEME_H
#define CSCHEME_H

#include <QGraphicsScene>

#include <QtXml/QDomDocument>

class QAction;

class CElement;
class CAlgorithmProto;
class CAlgorithmProtoMng;
class CAlgorithm;
class CPortal;
class CLink;

struct stSchemeDesc
{
	qreal width;
	qreal height;
};

class CScheme : public QGraphicsScene
{
	 Q_OBJECT
private:
	 bool m_newScheme;
	 bool m_modified;
	 QString m_fileName;

	 CAlgorithmProtoMng *m_algProtoMng;

     QDomElement variantToDomElement(QDomDocument domDoc, const QString &name,
                                     const QVariant &value);
	 QVariant domElementToVariant(const QDomElement &domElement);
	 void writeElementToXML(QDomDocument &doc, QDomElement &domElParent, CElement *element);
     CElement* readElementFromXML(const QDomDocument &domDoc, const QDomElement &domElElement,
                                  CElement *parentElement = 0);
public:
     explicit CScheme(const int &nomber, QObject *parent = 0);

     QString id(){return QString("Scheme");}
     const bool& isNewScheme() const{return m_newScheme;}
	 void setNewScheme(const bool &newScheme){m_newScheme = newScheme;}
     const bool& isModified() const{return m_modified;}
	 void setModified(const bool &modified);
     const QString& fileName() const{return m_fileName;}
	 void setFileName(const QString &fileName);

     void setAlgorithmProtoMng(CAlgorithmProtoMng *algorithmProtoMng);
	 CElement* createElement(const QString &typeID);
	 QList<CElement*> elements(const QString &typeID = QString());
     QList<CElement*> selectedElements();
     QList<CAlgorithm*> selectedAlgorithms();
	 CElement* element(const QString &id);
     QList<CAlgorithm*> algorithms();
     QList<CLink*> links();

	 bool checkXMLSchemeFormat(const QDomDocument &domDoc);
	 QDomDocument toXMLDom(const QList<CElement*> &elements, stSchemeDesc *schemeDesc = 0);
	 QList<CElement*> fromXMLDom(const QDomDocument &domDoc, stSchemeDesc *schemeDesc = 0);
private slots:
     void onSelectionChanged();
     void changeSchemeModified();
public slots:
	 void addElement(CElement *element);
	 void addElements(const QList<CElement*> &els);
	 void createAlgorithm(const QPointF &pos);
	 void addAlgorithm(CAlgorithmProto *algorithmProto, const QPointF &pos);
	 void addAlgorithm(CAlgorithm *algorithm, const QPointF &pos);
	 void addLink(const QString &firstPortalID, const QString &secondPortalID);
	 void addLink(CPortal *firstPortal, CPortal *secondPortal);
     void deleteSelected();
signals:
	 void elementAdded(CElement *element);
	 void fileNameChanged(QString fileName);
	 void algorithmsSelected(QList<CAlgorithm*> algorithms);
	 void schemeModified(bool modified);
};

#endif // CSCHEME_H
