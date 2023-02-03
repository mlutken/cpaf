#include "IntegerConstant.h"

using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(IntegerConstant,Integer)

IntegerConstant::IntegerConstant()
    : m_name("")
{
}

std::string IntegerConstant::do_docStringGet() const
{
    return do_nameGet();
}

void IntegerConstant::do_nameSet(const std::string& name)
{
    m_name = name;
}

std::string IntegerConstant::do_nameGet() const
{
    return m_name;
}

} // namespace phpdoc
