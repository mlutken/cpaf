#include "String.h"

#include "phpdoc_utils.h"

namespace phpdoc {

PHPDOC_NODE_CPP(String,Leaf)


std::string String::do_docStringGet() const
{
    return m_val;
}

bool String::do_parse(Iterator& first, const Iterator& last)
{
    m_val = toStdString(first, last);
}

} // namespace phpdoc
