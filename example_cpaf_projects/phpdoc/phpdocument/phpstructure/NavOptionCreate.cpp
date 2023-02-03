#include "NavOptionCreate.h"

namespace phpdoc {

PHPDOC_NODE_CPP(NavOptionCreate,FunctionCall)

NavOptionCreate::NavOptionCreate()
{
}

NavOptionCreate* NavOptionCreate::actionNameSet(const std::string& actionName)
{
    do_actionNameSet(actionName);
    return this;
}

bool NavOptionCreate::do_hasParameter(unsigned int /*parameterPos*/) const
{
    return false;
}

} // namespace phpdoc
