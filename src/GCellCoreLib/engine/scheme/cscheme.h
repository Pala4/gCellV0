#ifndef CSCHEME_H
#define CSCHEME_H

#include "gcellcorelib_global.h"

#include <QString>
#include <QMap>

#include "algdesc.h"

namespace gccore {

class CAlg;
class CAlgDescMng;

class GCELLCORELIBSHARED_EXPORT CScheme
{
private:
    int m_nomber;
    CAlgDescMng *m_algDescMng;
    QMap<QString, CAlg*> m_algs;
public:
    explicit CScheme(CAlgDescMng *algDescMng = nullptr, const int &nomber = -1);
    virtual ~CScheme();

    const int& nomber() const{return m_nomber;}
    void setNomber(const int &nomber){m_nomber = nomber;}

    QString id();

    QString createAlgorithm(const QString &algDescID); //returned alg id
    QString createAlgorithm(const AlgDesc &algDesc); //returned alg id

    CAlg* findAlgorithm(const QString &algID);

    bool linkAlgorithms(const QString &inAlgID, const QString &inAlgResultID,
                        const QString &outAlgID, const QString &outAlgArgumentID);
};

}

#endif // CSCHEME_H