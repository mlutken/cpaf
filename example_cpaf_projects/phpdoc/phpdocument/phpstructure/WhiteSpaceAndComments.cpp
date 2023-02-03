#include "WhiteSpaceAndComments.h"

#include "WhiteSpace.h"

namespace phpdoc {

PHPDOC_NODE_CPP(WhiteSpaceAndComments,Group)

WhiteSpaceAndComments::WhiteSpaceAndComments()
{
    appendNode(WhiteSpace::create());
}

WhiteSpaceAndComments* WhiteSpaceAndComments::whiteSpaceSet(const std::string& val)
{
    clear();
    appendNode(WhiteSpace::create()->valueSet(val));
    return this;
}

/** \todo Implement comment part of this function also!*/
void WhiteSpaceAndComments::do_valueSet(const std::string& val)
{
    whiteSpaceSet(val);
}

/** \todo Implement comment part of this function also!*/
bool WhiteSpaceAndComments::do_parse(Iterator& first, const Iterator& last)
{
    clear();
    WhiteSpace* ws = WhiteSpace::create();
    bool ret = ws->parse(first, last);
    appendNode(ws);
    return ret;
}

} // namespace phpdoc
