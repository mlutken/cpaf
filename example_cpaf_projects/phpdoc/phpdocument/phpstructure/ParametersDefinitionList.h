#ifndef PHPDOC_PARAMETERLIST_H
#define PHPDOC_PARAMETERLIST_H

#include "Group.h"

namespace phpdoc {

class ParametersDefinitionList : public Group
{
    PHPDOC_NODE_HDR(ParametersDefinitionList,Group)
public:
    ParametersDefinitionList();
};

} // namespace phpdoc

#endif // PHPDOC_PARAMETERLIST_H
