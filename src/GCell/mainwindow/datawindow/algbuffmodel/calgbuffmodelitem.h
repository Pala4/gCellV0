#ifndef CALGBUFFMODELITEM_H
#define CALGBUFFMODELITEM_H

#include <QObject>
#include <QStandardItem>

class CAlgorithm;
class CPortal;

class CAlgBuffModelItem : public QObject, public QStandardItem
{
	Q_OBJECT
public:
	enum AlgBuffModelItemType{Algorithm = QStandardItem::UserType + 1,
							  PortalNodeName,
							  Portal,
							  NoType};
private:
	CAlgorithm *m_alg;
	CPortal *m_portal;
	CAlgBuffModelItem::AlgBuffModelItemType m_itemType;

	void initCAlgBuffModelItem(void);
	void addRow(CAlgBuffModelItem *item);
public:
	CAlgBuffModelItem(CAlgorithm *alg, QObject *parent = 0);
	CAlgBuffModelItem(const QList<CPortal*> &portals, const QString &groupName, QObject *parent = 0);
	CAlgBuffModelItem(CPortal *portal, QObject *parent = 0);

	virtual int type(void) const{return m_itemType;}
	virtual void setData(const QVariant &value, int role = Qt::UserRole + 1);

	CAlgorithm* alg(void){return m_alg;}
	CPortal* portal(void){return m_portal;}
signals:
	void itemChanged(QStandardItem *item, int role);
};

#endif // CALGBUFFMODELITEM_H