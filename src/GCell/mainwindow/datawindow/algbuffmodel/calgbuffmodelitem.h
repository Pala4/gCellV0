#ifndef CALGBUFFMODELITEM_H
#define CALGBUFFMODELITEM_H

#include <QStandardItem>

class CAlgorithm;
class CPortal;

class CAlgBuffModelItem : public QStandardItem
{
public:
	enum AlgBuffModelItemType{Algorithm = QStandardItem::UserType + 1,
							  PortalNodeName,
							  Portal,
							  NoType};
private:
	CAlgorithm *m_alg;
	CPortal *m_portal;
	QString m_portalNodeName;
	CAlgBuffModelItem::AlgBuffModelItemType m_itemType;

	void initConstructor(void);
public:
	CAlgBuffModelItem(CAlgorithm *alg);
	CAlgBuffModelItem(const QString &portalNodeName);
	CAlgBuffModelItem(CPortal *portal);

	virtual int type(void) const{return m_itemType;}
};

#endif // CALGBUFFMODELITEM_H