#ifndef CALGDESCMNG_H
#define CALGDESCMNG_H

#include "gcellcorelib_global.h"

#include <QMap>
#include <QString>

#include "algdesc.h"

namespace gccore {

class GCELLCORELIBSHARED_EXPORT CAlgDescMng
{
private:
    QMap<QString, AlgDesc> m_algDescs;
public:
    CAlgDescMng();

    AlgDesc findDesc(const QString &id, bool &ok);
    bool contains(const AlgDesc &algDesc);
    void addDesc(const AlgDesc &algDesc);
};

}

#endif // CALGDESCMNG_H