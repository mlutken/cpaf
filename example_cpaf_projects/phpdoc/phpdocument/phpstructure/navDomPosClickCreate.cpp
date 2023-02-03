#include "navDomPosClickCreate.h"

#include <phpstructure/CallParameter.h>
#include <phpstructure/QuotedString.h>

using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(navDomPosClickCreate,NavOptionCreate)

navDomPosClickCreate::navDomPosClickCreate()
{
    functionNameSet("navDomPosClickCreate");
    parameterAdd (
                        CallParameter::create()->parameterSet(QuotedString::create()->valueSet(""))->whiteSpaceBeforeSet(" ")
            )->parameterAdd (
                        CallParameter::create()->parameterSet(QuotedString::create()->valueSet(""))->whiteSpaceBeforeSet(" ")->whiteSpaceAfterSet(" ")
            )->wsBeforeStatementSet("\t")
            ;
}


navDomPosClickCreate* navDomPosClickCreate::findNextSequenceSet(const string& sequence)
{
    CallParameter* cp = callParameter(SEQUENCE_POS);
    if (cp) {
        cp->parameter<QuotedString>()->valueSet(sequence);
    }
    return this;
}

string navDomPosClickCreate::findNextSequence() const
{
    CallParameter* cp = callParameter(SEQUENCE_POS);
    if (cp) {
        return cp->parameter<QuotedString>()->value();
    }
    return "";
}


void navDomPosClickCreate::do_valueSet(const std::string& val)
{
    actionNameSet(val);
}


std::string navDomPosClickCreate::do_valueGet() const
{
    return actionName();
}

bool navDomPosClickCreate::do_hasParameter(unsigned int parameterPos) const
{
    return  parameterPos == NavigatorConstants::OPTION_PARAMETER_COUNT_POS         ||
            parameterPos == NavigatorConstants::OPTION_PARAMETER_VALUE_POS         ||
            parameterPos == NavigatorConstants::OPTION_PARAMETER_SEARCH_FOR_POS;
}


void navDomPosClickCreate::do_actionNameSet(const string& actionName)
{
    CallParameter* cp = callParameter(ACTION_NAME_POS);
    if (cp) {
        cp->parameter<QuotedString>()->valueSet(actionName);
    }
}

string navDomPosClickCreate::do_actionNameGet() const
{
    CallParameter* cp = callParameter(ACTION_NAME_POS);
    if (cp) {
        return cp->parameter<QuotedString>()->value();
    }
    return "";
}

} // namespace phpdoc
