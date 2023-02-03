#ifndef PHPDOC_NAVIGATOR_H
#define PHPDOC_NAVIGATOR_H

#include "Group.h"

namespace phpdoc {

class Navigator : public Group
{
    PHPDOC_NODE_HDR(Navigator,Group)
public:
    Navigator();

};

} // namespace phpdoc

#endif // PHPDOC_NAVIGATOR_H
