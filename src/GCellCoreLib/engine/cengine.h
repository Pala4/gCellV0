#ifndef CENGINE_H
#define CENGINE_H

#include "gcellcorelib_global.h"

namespace gccore {

class CScheme;

class GCELLCORELIBSHARED_EXPORT CEngine
{
public:
    CEngine();

    void calcScheme(CScheme *scheme);
};

}

#endif // CENGINE_H