#include "navUrlLoadCreate.h"

#include <phpstructure/CallParameter.h>
#include <phpstructure/QuotedString.h>

using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(navUrlLoadCreate,NavOptionCreate)

navUrlLoadCreate::navUrlLoadCreate()
{
    functionNameSet("navUrlLoadCreate");
    parameterAdd (
                        CallParameter::create()->parameterSet(QuotedString::create()->valueSet(""))->whiteSpaceBeforeSet(" ")
            )->parameterAdd (
                        CallParameter::create()->parameterSet(QuotedString::create()->valueSet(""))->whiteSpaceBeforeSet(" ")->whiteSpaceAfterSet(" ")
            )->wsBeforeStatementSet("\t")
            ;
}



navUrlLoadCreate* navUrlLoadCreate::baseUrlSet(const string& baseUrl)
{
    CallParameter* cp = callParameter(BASE_URL_POS);
    if (cp) {
        cp->parameter<QuotedString>()->valueSet(baseUrl);
    }
    return this;
}

string navUrlLoadCreate::baseUrl() const
{
    CallParameter* cp = callParameter(BASE_URL_POS);
    if (cp) {
        return cp->parameter<QuotedString>()->value();
    }
    return "";
}


void navUrlLoadCreate::do_valueSet(const std::string& val)
{
    actionNameSet(val);
}


std::string navUrlLoadCreate::do_valueGet() const
{
    return actionName();
}

bool navUrlLoadCreate::do_hasParameter(unsigned int parameterPos) const
{
    return  parameterPos == NavigatorConstants::OPTION_PARAMETER_VALUE_POS         ||
            parameterPos == NavigatorConstants::OPTION_PARAMETER_SEARCH_FOR_POS;
}

void navUrlLoadCreate::do_actionNameSet(const string& actionName)
{
    CallParameter* cp = callParameter(ACTION_NAME_POS);
    if (cp) {
        cp->parameter<QuotedString>()->valueSet(actionName);
    }
}

string navUrlLoadCreate::do_actionNameGet() const
{
    CallParameter* cp = callParameter(ACTION_NAME_POS);
    if (cp) {
        return cp->parameter<QuotedString>()->value();
    }
    return "";
}

} // namespace phpdoc
