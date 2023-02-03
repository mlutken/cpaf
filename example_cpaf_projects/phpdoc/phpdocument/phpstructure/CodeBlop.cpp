#include "CodeBlop.h"

#include "String.h"
#include "WhiteSpaceAndComments.h"

using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(CodeBlop,Statement)

CodeBlop::CodeBlop()
{
    appendNode(WhiteSpaceAndComments::create()->valueSet(""));
    appendNode(String::create()->valueSet(""));
    appendNode(WhiteSpaceAndComments::create()->valueSet(""));
    endsWithSemicolonSet(false); // Statements ends with semicolon by default, but for codeblops it makes sense to assume it contains that itself if needed.
}

CodeBlop* CodeBlop::blopSet(const string& val)
{
    get<String>(BLOP_POS)->valueSet(val);
}

string CodeBlop::blop() const
{
    return get<String>(BLOP_POS)->value();
}

void CodeBlop::do_valueSet(const std::string& val)
{
    blopSet(val);
}

} // namespace phpdoc
