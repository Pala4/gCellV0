#ifndef CALGBUFFMODELITEM_H
#define CALGBUFFMODELITEM_H

#include <QObject>
#include <QStandardItem>

class CScheme;
class CAlgorithm;
class CPortal;

class CAlgBuffModelItem : public QObject, public QStandardItem
{
	Q_OBJECT
public:
    enum AlgBuffModelItemType{Scheme = QStandardItem::UserType + 1,
                              Algorithm,
							  PortalNodeName,
							  Portal,
							  NoType};
private:
	CAlgBuffModelItem::AlgBuffModelItemType m_itemType;
protected:
    void setItemType(const CAlgBuffModelItem::AlgBuffModelItemType &itemType);
public:
    explicit CAlgBuffModelItem(QObject *parent = 0);
    virtual ~CAlgBuffModelItem();

    virtual int type() const{return m_itemType;}
	virtual void setData(const QVariant &value, int role = Qt::UserRole + 1);

    void addRow(CAlgBuffModelItem *item);
    void clear();
public slots:
    void changeText(const QString &newText);
signals:
	void itemChanged(QStandardItem *item, int role);
};

class CGroupItem : public CAlgBuffModelItem
{
    Q_OBJECT
private:
    QString m_name;
public:
    explicit CGroupItem(const QString &name, QObject *parent = 0);

    const QString& name() const{return m_name;}
};

class CSchemeItem : public CAlgBuffModelItem
{
    Q_OBJECT
private:
    CScheme *m_scheme;

    bool contains(CAlgorithm *alg);
    int indexOf(CAlgorithm *alg);
public:
    explicit CSchemeItem(CScheme *scheme, QObject *parent = 0);

    CScheme* scheme(){return m_scheme;}

    void addAlgs(const QList<CAlgorithm*> &algs);
    void addAlg(CAlgorithm *alg);
    void remAlgs(const QList<CAlgorithm*> &algs = QList<CAlgorithm*>());
    void remAlg(CAlgorithm *alg);
private slots:
    void onSelectionChanged();
};

class CAlgItem : public CAlgBuffModelItem
{
    Q_OBJECT
private:
    CAlgorithm *m_alg;
    CGroupItem *m_argGroup;
    CGroupItem *m_resGroup;

    bool contains(CGroupItem *group, CPortal *portal);
    int indexOf(CGroupItem *group, CPortal *portal);
public:
    explicit CAlgItem(CAlgorithm *alg, QObject *parent = 0);

    CAlgorithm* alg(){return m_alg;}

    void addPortals(CGroupItem *group, const QList<CPortal*> &portals);
    void addPortal(CGroupItem *group, CPortal *portal);
    void remPortals(CGroupItem *group, const QList<CPortal*> &portals = QList<CPortal*>());
    void remPortal(CGroupItem *group, CPortal *portal);
private slots:
    void onPortalAdded(CPortal *portal);
};

class CPortalItem : public CAlgBuffModelItem
{
    Q_OBJECT
private:
    CPortal *m_portal;
public:
    explicit CPortalItem(CPortal *portal, QObject *parent = 0);

    CPortal* portal(){return m_portal;}
};

#endif // CALGBUFFMODELITEM_H
