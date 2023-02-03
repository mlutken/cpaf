#include "navDropDownSelectCreate.h"

#include <phpstructure/CallParameter.h>
#include <phpstructure/QuotedString.h>

using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(navDropDownSelectCreate,NavOptionCreate)


navDropDownSelectCreate::navDropDownSelectCreate()
{
    functionNameSet("navDropDownSelectCreate");
    parameterAdd (
                CallParameter::create()->parameterSet(QuotedString::create()->valueSet(""))->whiteSpaceBeforeSet("")
            )->wsBeforeStatementSet("\t")
            ;
}


void navDropDownSelectCreate::do_valueSet(const std::string& val)
{
    actionNameSet(val);
}


std::string navDropDownSelectCreate::do_valueGet() const
{
    return actionName();
}

bool navDropDownSelectCreate::do_hasParameter(unsigned int parameterPos) const
{
    return parameterPos == NavigatorConstants::OPTION_PARAMETER_COUNT_POS;
}

void navDropDownSelectCreate::do_actionNameSet(const string& actionName)
{
    CallParameter* cp = callParameter(ACTION_NAME_POS);
    if (cp) {
        cp->parameter<QuotedString>()->valueSet(actionName);
    }
}

string navDropDownSelectCreate::do_actionNameGet() const
{
    CallParameter* cp = callParameter(ACTION_NAME_POS);
    if (cp) {
        return cp->parameter<QuotedString>()->value();
    }
    return "";
}


} // namespace phpdoc
