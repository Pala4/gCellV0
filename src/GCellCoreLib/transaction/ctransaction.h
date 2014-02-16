#ifndef CTRANSACTION_H
#define CTRANSACTION_H

#include "gcellcorelib_global.h"

#include <QObject>

class GCELLCORELIBSHARED_EXPORT CTransaction : public QObject
{
    Q_OBJECT
private:
    enum TransactionType{Query, Respons};

    CTransaction::TransactionType m_transactionType;

    QObject *m_queryReceiver;
    QObject *m_responsReceiver;
    QString m_query;
    int m_cmdID;

    QString m_respons;

    void setTransactionType(const CTransaction::TransactionType &transactionType);
public:
    explicit CTransaction(QObject *queryReceiver, QObject *responsReceiver, const QString &query,
                          const int &cmdID, QObject *parent = 0);

    QObject* queryReceiver() {return m_queryReceiver;}
    void setQueryReceiver(QObject *queryReceiver);
    QObject* responsReceiver() {return m_responsReceiver;}
    void setResponsReceiver(QObject *responsReceiver);
    const QString& query() const{return m_query;}
    void setQuery(const QString &query){m_query = query;}
    const int& cmdID() const{return m_cmdID;}
    void setCmdID(const int &cmdID){m_cmdID = cmdID;}

    const QString& respons() const{return m_respons;}
    void setRespons(const QString &respons){m_respons = respons;}

    bool isQuery() const{return (m_transactionType == CTransaction::Query);}
    bool isRespons() const{return (m_transactionType == CTransaction::Respons);}
private slots:
    void onQueryReceiverDestroyed(){m_queryReceiver = nullptr;}
    void onResponsReceiverDestroyed(){m_responsReceiver = nullptr;}
public slots:
    void sendQuery();
    void sendRespons();
    void sendRespons(const QString &respons);
};

#endif // CTRANSACTION_H
