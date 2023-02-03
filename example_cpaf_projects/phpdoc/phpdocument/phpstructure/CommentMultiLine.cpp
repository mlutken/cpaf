#include "CommentMultiLine.h"

using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(CommentMultiLine,Comment)

std::string CommentMultiLine::do_docStringGet() const
{
    string s("/*");
    s.append(m_val);
    s.append("*/");
    return s;
}

void CommentMultiLine::do_valueSet(const string& val)
{
    m_val = val;
}


} // namespace phpdoc
