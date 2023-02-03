#include "CallParameter.h"

#include "String.h"
#include "QuotedString.h"
#include "Integer.h"
#include "WhiteSpaceAndComments.h"

using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(CallParameter,Group)


CallParameter::CallParameter()
{
    appendNode(WhiteSpaceAndComments::create()->valueSet(""));
    appendNode(String::create()->valueSet(""));
    appendNode(WhiteSpaceAndComments::create()->valueSet(""));
}

CallParameter* CallParameter::parameterSet(Node* pNode)
{
    replaceNode(pNode, PARAMETER_POS);
    return this;
}

CallParameter* CallParameter::whiteSpaceSet(WhiteSpaceAndComments* wsBefore, WhiteSpaceAndComments* wsAfter)
{
    replaceNode(wsBefore, WS_0_POS);
    replaceNode(wsAfter, WS_1_POS);
    return this;
}

CallParameter* CallParameter::whiteSpaceBeforeSet(WhiteSpaceAndComments* wsBefore)
{
    replaceNode(wsBefore, WS_0_POS);
    return this;
}

CallParameter* CallParameter::whiteSpaceAfterSet(WhiteSpaceAndComments* wsAfter)
{
    replaceNode(wsAfter, WS_1_POS);
    return this;
}

CallParameter* CallParameter::whiteSpaceBeforeSet(const string& wsBefore)
{
    return whiteSpaceBeforeSet(WhiteSpaceAndComments::create()->whiteSpaceSet(wsBefore));
}

CallParameter* CallParameter::whiteSpaceAfterSet(const string& wsAfter)
{
    return whiteSpaceAfterSet(WhiteSpaceAndComments::create()->whiteSpaceSet(wsAfter));
}

CallParameter* CallParameter::parameterQuotedStringSet(const string& value)
{
    parameterSet( QuotedString::create()->valueSet(value) );
    return this;
}

CallParameter* CallParameter::parameterQuotedStringSet(const string& value, QuotedStringBase::QuoteTypeE quoteType)
{
    parameterSet(QuotedString::create()->quoteTypeSet(quoteType)->valueSet(value));
    return this;
}

string CallParameter::parameterQuotedString() const
{
    QuotedString* qs = parameter<QuotedString>();
    if (qs) return qs->value();
    return "";
}

CallParameter* CallParameter::parameterIntSet(int value)
{
    parameterSet( Integer::create()->valueIntSet(value) );
    return this;
}

CallParameter* CallParameter::parameterIntSet(const string& value)
{
    parameterSet( Integer::create()->valueSet(value) );
    return this;
}

int CallParameter::parameterInt() const
{
    Integer* par = parameter<Integer>();
    if (par) return par->valueInt();
    return 0;
}

Node* CallParameter::parameter() const
{
    return getNode(PARAMETER_POS);
}

void CallParameter::do_valueSet(const string &val)
{

}

bool CallParameter::parseParameter(Iterator& first, const Iterator& last)
{

}


} // namespace phpdoc
