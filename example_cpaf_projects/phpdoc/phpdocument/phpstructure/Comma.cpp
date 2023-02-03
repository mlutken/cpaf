#include "Comma.h"

namespace phpdoc {

PHPDOC_NODE_CPP(Comma,Leaf)


std::string Comma::do_docStringGet() const
{
    return ",";
}

} // namespace phpdoc
