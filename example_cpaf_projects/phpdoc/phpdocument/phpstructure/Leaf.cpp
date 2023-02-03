#include "Leaf.h"

namespace phpdoc {

PHPDOC_NODE_CPP(Leaf, Node)

bool Leaf::do_isLeafGet    () const
{
    return true;
}

void Leaf::do_search(NodeVecT& results, const NodeSearchFunT& searchFun)
{
    if (searchFun(this)) results.push_back(this);
}


} // END namespace phpdoc
