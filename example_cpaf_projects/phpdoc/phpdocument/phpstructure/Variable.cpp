#include "Variable.h"

using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(Variable,Identifier)


std::string Variable::do_docStringGet() const
{
    return string("$") + identifierName();
}

} // namespace phpdoc
