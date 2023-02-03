#include "Integer.h"

#include <cpaf_string_utils.hpp>

using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(Integer,Leaf)

Integer::Integer()
    : m_val (0)
{
}

Integer* Integer::valueIntSet(int val)
{
    m_val = val;
    return this;
}

string Integer::do_docStringGet() const
{
    return do_valueGet();
}

void Integer::do_valueSet(const string &val)
{
    m_val = cpaf::to_int(val);
}

string Integer::do_valueGet() const
{
    return cpaf::to_string(m_val);
}

} // namespace phpdoc
