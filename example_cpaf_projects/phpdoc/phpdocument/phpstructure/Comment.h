#ifndef PHPDOC_COMMENT_H
#define PHPDOC_COMMENT_H

#include "Leaf.h"

namespace phpdoc {

class Comment : public Leaf
{
    PHPDOC_NODE_HDR(Comment, Leaf)
public:
};

} // namespace phpdoc

#endif // PHPDOC_COMMENT_H
