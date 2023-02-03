#include "globals_utils.h"

#include <phpstructure/Globals.h>
#include <phpstructure/Node.h>

namespace phpdoc {

doctypes::Type classNameToTypes(const std::string& className)
{
    if (className == "" ) return doctypes::All;
    return g().fac().create(className)->docTypes();
}

} // namespace phpdoc
