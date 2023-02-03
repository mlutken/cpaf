#ifndef PHPDOC_STATEMENTSBLOCK_H
#define PHPDOC_STATEMENTSBLOCK_H

#include "Group.h"

namespace phpdoc {

class StatementsBlock : public Group
{
PHPDOC_NODE_HDR(StatementsBlock,Group)

public:
    StatementsBlock();
};

} // namespace phpdoc

#endif // PHPDOC_STATEMENTSBLOCK_H
