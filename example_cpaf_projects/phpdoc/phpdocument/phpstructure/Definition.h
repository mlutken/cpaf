#ifndef PHPDOC_DEFINITION_H
#define PHPDOC_DEFINITION_H

#include "Group.h"

namespace phpdoc {

class Definition : public Group
{
    PHPDOC_NODE_HDR(Definition,Group)
public:
    Definition();
};

} // namespace phpdoc

#endif // PHPDOC_DEFINITION_H
