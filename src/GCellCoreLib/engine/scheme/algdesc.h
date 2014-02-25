#ifndef ALGDESC_H
#define ALGDESC_H

#include "gcellcorelib_global.h"

#include <string>
#include <map>

#include "portaldesc.h"

namespace gccore {

struct GCELLCORELIBSHARED_EXPORT AlgDesc
{
    AlgDesc(){}
    std::string id;
    std::string name;

    std::map<std::string, PortalDesc> arguments;
    std::map<std::string, PortalDesc> results;
};

}

#endif // ALGDESC_H