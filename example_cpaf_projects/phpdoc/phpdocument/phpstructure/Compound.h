#ifndef PHPDOC_COMPOUND_H
#define PHPDOC_COMPOUND_H

#include "Node.h"

namespace phpdoc {

/** Base container type node supporting children. */
class Compound : public Node
{
public:
    PHPDOC_NODE_HDR(Compound,Node)

private:
    virtual std::string do_docStringGet () const { return ""; }
    virtual void        do_valueSet     (const std::string& ) {}

    virtual void        do_search       ( NodeVecT& results, const NodeSearchFunT& searchFun ) override;

};

} // namespace phpdoc

#endif // PHPDOC_COMPOUND_H
