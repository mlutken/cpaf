#include "Semicolon.h"

namespace phpdoc {

PHPDOC_NODE_CPP(Semicolon,Leaf)


std::string Semicolon::do_docStringGet() const
{
    return ";";
}

} // namespace phpdoc
