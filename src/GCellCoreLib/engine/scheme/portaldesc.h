#ifndef PORTALDESC_H
#define PORTALDESC_H

#include "gcellcorelib_global.h"

#include <string>

namespace gccore {

struct GCELLCORELIBSHARED_EXPORT PortalDesc
{
    enum PortalType{Argument, Result};

    PortalDesc() : portalType(PortalDesc::Argument) {}

    PortalType portalType;
    std::string name;
};

}

#endif // PORTALDESC_H