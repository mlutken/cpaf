#include "Identifier.h"

using namespace std;

namespace phpdoc {


PHPDOC_NODE_CPP(Identifier,Leaf)


std::string Identifier::do_docStringGet() const
{
    return m_val;
}

} // namespace phpdoc
