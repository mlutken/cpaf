#include "CommentSingleLine.h"

using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(CommentSingleLine,Leaf)

std::string CommentSingleLine::do_docStringGet() const
{
    string s("//");
    s.append(m_val);
    s.append("\n");
    return s;
}

void CommentSingleLine::do_valueSet(const string& val)
{
    m_val = val;
}


} // namespace phpdoc
