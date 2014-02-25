#ifndef CWORKSPACE_H
#define CWORKSPACE_H

#include "gcellcorelib_global.h"

#include <QObject>

#include <QMap>

namespace gccore {

class CAlgDescMng;
class CScheme;
class CAlg;
class CEngine;

class GCELLCORELIBSHARED_EXPORT CSchemeMng : public QObject
{
    Q_OBJECT
private:
    CAlgDescMng *m_algDescMng;
    QMap<QString, CScheme*> m_schemes;
    CEngine *m_engine;
public:
    explicit CSchemeMng(QObject *parent = 0);
    virtual ~CSchemeMng();

    CScheme* findScheme(const QString &schemeID);
public slots:
    QString createScheme(); //schemeID returned
    QString createAlgorithm(const QString &schemeID, const QString &algID); //algID returned
    void linkAlgorithms(const QString &schemeID,
                        const QString &inAlgID, const QString &inAlgResultID,
                        const QString &outAlgID, const QString &outAlgArgumentID);
    void calcScheme(const QString &schemeID);
};

}

#endif // CWORKSPACE_H