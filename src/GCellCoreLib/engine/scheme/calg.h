#ifndef CALG_H
#define CALG_H

#include "gcellcorelib_global.h"

#include <QMap>
#include <QString>

#include "algDesc.h"

namespace gccore {

class CScheme;
class CPortal;

class GCELLCORELIBSHARED_EXPORT CAlg
{
private:
    AlgDesc m_algDesc;
    CScheme *m_scheme;
    int m_nomber;

    QMap<QString, CPortal*> m_arguments;
    QMap<QString, CPortal*> m_results;
public:
    explicit CAlg(const AlgDesc &algDesc, CScheme *scheme = nullptr, const int &nomber = -1);
    virtual ~CAlg();

    const AlgDesc& algDesc() const{return m_algDesc;}
    void setAlgDesc(const AlgDesc &algDesc);
    CScheme* scheme(){return m_scheme;}
    void setScheme(CScheme *scheme){m_scheme = scheme;}
    const int& nomber() const{return m_nomber;}
    void setNomber(const int &nomber){m_nomber = nomber;}

    const QMap<QString, CPortal*>& arguments() const{return m_arguments;}
    const QMap<QString, CPortal*>& results() const{return m_results;}
    void createPortals(const std::map<std::string, PortalDesc> &portalDescs);
    CPortal* findArgument(const QString &argumentID);
    CPortal* findResult(const QString &resultID);
    void clearArguments();
    void clearResults();

    QString id();
};

}

#endif // CALG_H