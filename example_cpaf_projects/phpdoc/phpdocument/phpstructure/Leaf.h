#ifndef LEAF_H
#define LEAF_H

#include "Node.h"


namespace phpdoc {

class Leaf : public Node
{
public:
    PHPDOC_NODE_HDR(Leaf, Node)

private:
    virtual bool        do_isLeafGet    () const override ;
    virtual std::string do_docStringGet () const override { return ""; }
    virtual void        do_valueSet     (const std::string& ) override {}

    virtual void        do_search       ( NodeVecT& results, const NodeSearchFunT& searchFun ) override;

};

} // END namespace phpdoc

#endif // LEAF_H
