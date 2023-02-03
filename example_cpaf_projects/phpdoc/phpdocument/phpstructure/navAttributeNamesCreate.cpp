#include "navAttributeNamesCreate.h"

#include "QuotedStringList.h"
#include "CallParameter.h"
#include "ParametersCallList.h"

using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(navAttributeNamesCreate,FunctionCall)

navAttributeNamesCreate::navAttributeNamesCreate()
{
    functionNameSet("navAttributeNamesCreate");
    parameterAdd ( CallParameter::create()->parameterSet(QuotedStringList::create()) );
    wsBeforeStatementSet("\t");
    attributeNames()->reserveAndAssign(4,"");
}

QuotedStringList* navAttributeNamesCreate::attributeNames() const
{
    CallParameter* cp = callParameter(ATTRIBUTES_LIST_POS);
    if (cp) {
        return cp->parameter<QuotedStringList>();
    }
    return nullptr;
}

void navAttributeNamesCreate::do_valueSet(const string &val)
{
    QuotedStringList* sl = attributeNames();
    if (sl) sl->valueSet(val);
}

string navAttributeNamesCreate::do_valueGet() const
{
    QuotedStringList* sl = attributeNames();
    if (sl) return sl->value();
    return "";
}


} // namespace phpdoc
