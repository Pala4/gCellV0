#ifndef CPORTAL_H
#define CPORTAL_H

#include "gcellcorelib_global.h"

#include <QMap>

#include "portaldesc.h"

namespace gccore {

class CAlg;

class GCELLCORELIBSHARED_EXPORT CPortal
{
private:
    PortalDesc m_portalDesc;
    CAlg *m_alg;
    int m_nomber;
    QMap<QString, CPortal*> m_linkedPortals;
public:
    explicit CPortal(const PortalDesc &portalDesc, CAlg *alg = nullptr, const int &nomber = -1);

    const PortalDesc& portalDesc() const{return m_portalDesc;}
    void setPortalDesc(const PortalDesc &portalDesc);
    CAlg* alg() const{return m_alg;}
    void setAlg(CAlg *alg){m_alg = alg;}
    const int& nomber() const{return m_nomber;}
    void setNomber(const int &nomber){m_nomber = nomber;}
    const QMap<QString, CPortal*>& linkedPortals() const{return m_linkedPortals;}
    bool linkToPortal(CPortal *portal);

    QString id();
};

}

#endif // CPORTAL_H