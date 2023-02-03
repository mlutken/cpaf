#include "QuotedString.h"

#include "WhiteSpaceAndComments.h"
#include "String.h"

using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(QuotedString,QuotedStringBase)

QuotedString::QuotedString()
{
    appendNode(WhiteSpaceAndComments::create()->valueSet(""));
    appendNode(String::create()->valueSet(""));
    appendNode(WhiteSpaceAndComments::create()->valueSet(""));
}

string QuotedString::do_docStringGet() const
{
    if (DoubleQuoteE == quoteType())    return string("\"") + value() + "\"";
    else                                return string("'") + value() + "'";
}

void QuotedString::do_valueSet(const string &val)
{
    get<String>(STRING_POS)->valueSet(val);
}

string QuotedString::do_valueGet() const
{
    return get<String>(STRING_POS)->value();
}

bool QuotedString::do_parse(Iterator& first, const Iterator& last)
{

}



} // namespace phpdoc
