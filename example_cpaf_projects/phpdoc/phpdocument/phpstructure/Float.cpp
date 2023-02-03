#include "Float.h"
#include <cpaf_string_utils.hpp>
#include <phpstructure/Globals.h>

using namespace std;

namespace phpdoc {



PHPDOC_NODE_CPP(Float,Leaf)


Float::Float()
    : m_val(0)
{
}

string Float::do_docStringGet() const
{
    return do_valueGet();
}

void Float::do_valueSet(const string &val)
{
    m_val = cpaf::to_float(val);
}

string Float::do_valueGet() const
{
    return cpaf::to_string(m_val);
}

} // namespace phpdoc
