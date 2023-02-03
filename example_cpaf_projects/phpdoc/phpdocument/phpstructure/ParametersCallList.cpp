#include "ParametersCallList.h"

#include <boost/foreach.hpp>
#include "Integer.h"

using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(ParametersCallList,Group)

ParametersCallList::ParametersCallList()
{
}



ParametersCallList* ParametersCallList::parameterAdd(CallParameter* pCallParameter)
{
    appendNode(pCallParameter);
    return this;
}

ParametersCallList* ParametersCallList::parameterSet(CallParameter* pCallParameter, unsigned int pos)
{
    if (positionValid(pos)) replaceNode(pCallParameter, pos);
    return this;
}


CallParameter* ParametersCallList::callParameter(unsigned int pos) const
{
    if ( positionValid(pos)) return get<CallParameter>(pos);
    return 0;
}



ParametersCallList* ParametersCallList::parameterQuotedStringSet(const string& value, unsigned int pos)
{
    CallParameter* cp = callParameter(pos);
    if (cp) cp->parameterQuotedStringSet(value);
    return this;
}

ParametersCallList* ParametersCallList::parameterQuotedStringSet(const string& value, unsigned int pos, QuotedStringBase::QuoteTypeE quoteType )
{
    CallParameter* cp = callParameter(pos);
    if (cp) cp->parameterQuotedStringSet(value, quoteType);
    return this;
}


string ParametersCallList::parameterQuotedString( unsigned int pos ) const
{
    QuotedString* qs = parameter<QuotedString>(pos);
    if (qs) return qs->value();
    return "";
}

ParametersCallList* ParametersCallList::parameterIntSet(int value, unsigned int pos)
{
    CallParameter* cp = callParameter(pos);
    if (cp) cp->parameterIntSet(value);
    return this;
}

ParametersCallList* ParametersCallList::parameterIntSet(const string& value, unsigned int pos)
{
    CallParameter* cp = callParameter(pos);
    if (cp) cp->parameterIntSet(value);
    return this;
}

int ParametersCallList::parameterInt(unsigned int pos) const
{
    Integer* par = parameter<Integer>(pos);
    if (par) return par->valueInt();
    return 0;
}

ParametersCallList* ParametersCallList::allParametersEnabledSet(bool enabled)
{
    for (Node* node : children()) {
        node->enabledSet(enabled);
    }
    return this;
}

ParametersCallList* ParametersCallList::parameterEnabledSet(unsigned int pos, bool enabled)
{
    Node* par = getNode(pos);
    if (par) par->enabledSet(enabled);
    return this;
}

bool ParametersCallList::parameterEnabled(unsigned int pos) const
{
    Node* par = getNode(pos);
    if (par) return par->enabled();
    return false;
}

unsigned int ParametersCallList::enabledParametersCount() const
{
    unsigned int n = 0;
    for (Node* node : children()) {
        if (node->enabled()) n++;
    }
    return n;
}


std::string ParametersCallList::do_docStringGet () const
{
    string s;
    int i = 0;
    for (Node* node : children()) {
        if ( node->enabled()) {
            if ( i > 0) s += ",";
            i++;
            s += node->docString();
        }
    }
    return s;
}

} // namespace phpdoc
