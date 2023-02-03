#include "navJavascriptExecuteCreate.h"

#include <phpstructure/CallParameter.h>
#include <phpstructure/QuotedString.h>

using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(navJavascriptExecuteCreate,NavOptionCreate)

navJavascriptExecuteCreate::navJavascriptExecuteCreate()
{
    functionNameSet("navJavascriptExecuteCreate");
    parameterAdd (
                CallParameter::create()->parameterSet(QuotedString::create()->valueSet(""))->whiteSpaceBeforeSet("")
            )->wsBeforeStatementSet("\t")
            ;
}

void navJavascriptExecuteCreate::do_valueSet(const std::string& val)
{
    actionNameSet(val);
}


std::string navJavascriptExecuteCreate::do_valueGet() const
{
    return actionName();
}

bool navJavascriptExecuteCreate::do_hasParameter(unsigned int parameterPos) const
{
    return  parameterPos == NavigatorConstants::OPTION_PARAMETER_VALUE_POS         ||
            parameterPos == NavigatorConstants::OPTION_PARAMETER_SEARCH_FOR_POS;
}


void navJavascriptExecuteCreate::do_actionNameSet(const string& actionName)
{
    CallParameter* cp = callParameter(ACTION_NAME_POS);
    if (cp) {
        cp->parameter<QuotedString>()->valueSet(actionName);
    }
}

string navJavascriptExecuteCreate::do_actionNameGet() const
{
    CallParameter* cp = callParameter(ACTION_NAME_POS);
    if (cp) {
        return cp->parameter<QuotedString>()->value();
    }
    return "";
}

} // namespace phpdoc
