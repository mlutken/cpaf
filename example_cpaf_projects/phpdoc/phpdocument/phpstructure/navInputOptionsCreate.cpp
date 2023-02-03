#include "navInputOptionsCreate.h"

#include <phpstructure/CallParameter.h>
#include <phpstructure/QuotedString.h>

using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(navInputOptionsCreate,NavOptionCreate)

navInputOptionsCreate::navInputOptionsCreate()
{
    functionNameSet("navInputOptionsCreate");
    parameterAdd (
                CallParameter::create()->parameterSet(QuotedString::create()->valueSet(""))->whiteSpaceBeforeSet("")
            )->wsBeforeStatementSet("\t")
            ;
}

void navInputOptionsCreate::do_valueSet(const std::string& val)
{
    actionNameSet(val);
}


std::string navInputOptionsCreate::do_valueGet() const
{
    return actionName();
}

bool navInputOptionsCreate::do_hasParameter(unsigned int parameterPos) const
{
    return  parameterPos == NavigatorConstants::OPTION_PARAMETER_VALUE_POS;
}

void navInputOptionsCreate::do_actionNameSet(const string& actionName)
{
    CallParameter* cp = callParameter(ACTION_NAME_POS);
    if (cp) {
        cp->parameter<QuotedString>()->valueSet(actionName);
    }
}

string navInputOptionsCreate::do_actionNameGet() const
{
    CallParameter* cp = callParameter(ACTION_NAME_POS);
    if (cp) {
        return cp->parameter<QuotedString>()->value();
    }
    return "";
}

} // namespace phpdoc
