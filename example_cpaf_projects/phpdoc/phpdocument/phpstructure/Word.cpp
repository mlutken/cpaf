#include "Word.h"

using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(Word,Leaf)


std::string Word::do_docStringGet() const
{
    return m_val;
}


} // namespace phpdoc
