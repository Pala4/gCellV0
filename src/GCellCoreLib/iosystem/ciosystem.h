#ifndef CIOSYSTEM_H
#define CIOSYSTEM_H

#include "gcellcorelib_global.h"

#include "cobject.h"

#include <QMap>

class GCELLCORELIBSHARED_EXPORT CIOSystem : public CObject
{
    Q_OBJECT
private:
    QMap<QString, CTransaction*> m_transactions;
protected:
    bool processForwardQuery(const QString &forwardQuery);
public:
    explicit CIOSystem(QObject *parent = 0);

    CTransaction* registerTransaction(QObject *queryReceiver, QObject *responsReceiver,
                                      const QString &query, const int &cmdID);
private slots:
    void onTransactionDestroyed(QObject *objTransaction);
};

#endif // CIOSYSTEM_H